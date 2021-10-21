#!/bin/bash
#
# Copyright 2016 leenjewel
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# read -n1 -p "Press any key to continue..."

set -u

source ./build-ios-common.sh

TOOLS_ROOT=$(pwd)

SOURCE="$0"
while [ -h "$SOURCE" ]; do
    DIR="$(cd -P "$(dirname "$SOURCE")" && pwd)"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
pwd_path="$(cd -P "$(dirname "$SOURCE")" && pwd)"

echo pwd_path=${pwd_path}
echo TOOLS_ROOT=${TOOLS_ROOT}

# openssl-1.1.0f has a configure bug
# openssl-1.1.1d has fix configure bug
LIB_VERSION="OpenSSL_1_1_1d"
LIB_NAME="openssl-1.1.1d"
LIB_DEST_DIR="${pwd_path}/build/ios/openssl-universal/lib"

init_log_color

echo "https://www.openssl.org/source/${LIB_NAME}.tar.gz"

# https://github.com/openssl/openssl/archive/OpenSSL_1_1_1d.tar.gz
# https://github.com/openssl/openssl/archive/OpenSSL_1_1_1f.tar.gz
DEVELOPER=$(xcode-select -print-path)
SDK_VERSION=$(xcrun -sdk iphoneos --show-sdk-version)
rm -rf "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || curl -L -o ${LIB_NAME}.tar.gz https://www.openssl.org/source/${LIB_NAME}.tar.gz -s
[ -f "${LIB_NAME}.tar.gz" ] || log_error "openssl download error!"

function configure_make() {

    ARCH=$1
    SDK=$2
    PLATFORM=$3

    log_info "configure $ARCH start..."

    if [ -d "${LIB_NAME}" ]; then
        rm -fr "${LIB_NAME}"
    fi
    tar xfz "${LIB_NAME}.tar.gz"
    pushd .
    cd "${LIB_NAME}"

    export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
    export CROSS_SDK="${PLATFORM}${SDK_VERSION}.sdk"

    if [ ! -d ${CROSS_TOP}/SDKs/${CROSS_SDK} ]; then
        log_error "ERROR: iOS SDK version:'${SDK_VERSION}' incorrect, SDK in your system is:"
        xcodebuild -showsdks | grep iOS
        exit -1
    fi

    PREFIX_DIR="${pwd_path}/build/ios/${ARCH}"
    if [ -d "${PREFIX_DIR}" ]; then
        rm -fr "${PREFIX_DIR}"
    fi
    mkdir -p "${PREFIX_DIR}"

    OUTPUT_ROOT=${TOOLS_ROOT}/build/ios/${ARCH}
    mkdir -p ${OUTPUT_ROOT}/log

    set_ios_cpu_feature "openssl" "${ARCH}" "${IOS_MIN_TARGET}" "${CROSS_TOP}/SDKs/${CROSS_SDK}"
    
    ios_printf_global_params "$ARCH" "$SDK" "$PLATFORM" "$PREFIX_DIR" "$OUTPUT_ROOT"

    unset IPHONEOS_DEPLOYMENT_TARGET

    if [[ "${ARCH}" == "x86_64" ]]; then

        # openssl1.1.1d can be set normally, 1.1.0f does not take effect
        ./Configure darwin64-x86_64-cc no-shared --prefix="${PREFIX_DIR}"

    elif [[ "${ARCH}" == "armv7" ]]; then

        # openssl1.1.1d can be set normally, 1.1.0f does not take effect
        ./Configure iphoneos-cross no-shared --prefix="${PREFIX_DIR}" enable-ssl3 enable-ssl3-method
        sed -ie "s!-fno-common!-fno-common -fembed-bitcode !" "Makefile"

    elif [[ "${ARCH}" == "arm64" ]]; then

        # openssl1.1.1d can be set normally, 1.1.0f does not take effect
        ./Configure iphoneos-cross no-shared --prefix="${PREFIX_DIR}" enable-ssl3 enable-ssl3-method
        sed -ie "s!-fno-common!-fno-common -fembed-bitcode !" "Makefile"

    elif [[ "${ARCH}" == "i386" ]]; then

        # openssl1.1.1d can be set normally, 1.1.0f does not take effect
        ./Configure darwin-i386-cc no-shared --prefix="${PREFIX_DIR}" enable-ssl3 enable-ssl3-method
        sed -ie "s!-fno-common!-fno-common -fembed-bitcode !" "Makefile"

    else
        log_error "not support" && exit 1
    fi

    log_info "make $ARCH start..."

    make clean >"${OUTPUT_ROOT}/log/${ARCH}.log"
    if make -j8 >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1; then
        make install_sw >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
        make install_ssldirs >>"${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
    fi

    popd
}

log_info "${PLATFORM_TYPE} ${LIB_NAME} start..."

for ((i = 0; i < ${#ARCHS[@]}; i++)); do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        log_info "configure_make ${ARCHS[i]}" "${SDKS[i]}" "${PLATFORMS[i]}"
        configure_make "${ARCHS[i]}" "${SDKS[i]}" "${PLATFORMS[i]}"
    fi
done

log_info "lipo start..."

function lipo_library() {
    LIB_SRC=$1
    LIB_DST=$2
    LIB_PATHS=("${ARCHS[@]/#/${pwd_path}/build/ios/}")
    LIB_PATHS=("${LIB_PATHS[@]/%//lib/${LIB_SRC}}")
    lipo ${LIB_PATHS[@]} -create -output "${LIB_DST}"
}

function copy_include() {
    DST=$1
    if [ -d "${pwd_path}/build/ios/x86_64/include" ]; then
        cp -r "${pwd_path}/build/ios/x86_64/include"  "${DST}"
    elif [ -d "${pwd_path}/build/ios/armv7/include" ]; then
        cp -r "${pwd_path}/build/ios/armv7/include"  "${DST}"
    elif [ -d "${pwd_path}/build/ios/arm64/include" ]; then
        cp -r "${pwd_path}/build/ios/arm64/include"  "${DST}"
    elif [ -d "${pwd_path}/build/ios/i386/include" ]; then
        cp -r "${pwd_path}/build/ios/i386/include"  "${DST}"  
    fi
}

mkdir -p "${LIB_DEST_DIR}"
lipo_library "libcrypto.a" "${LIB_DEST_DIR}/libcrypto.a"
lipo_library "libssl.a" "${LIB_DEST_DIR}/libssl.a"
copy_include "${LIB_DEST_DIR}/../"

log_info "${PLATFORM_TYPE} ${LIB_NAME} end..."
