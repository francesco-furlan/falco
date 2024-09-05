# SPDX-License-Identifier: Apache-2.0
#
# Copyright (C) 2023 The Falco Authors.
#
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
#

file(GLOB patches "*.patch")
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/libs-patches/)
foreach(p ${patches})
    configure_file("${p}" "${PROJECT_BINARY_DIR}/libs-patches/" @ONLY)
endforeach()

# Add a PATCH_COMMAND to libs grpc.cmake
# to fixup emitted -march by abseil-cpp cmake module,
# making it use a name understood by zig for arm64.
# See https://github.com/abseil/abseil-cpp/blob/master/absl/copts/GENERATED_AbseilCopts.cmake#L226.
execute_process(
        COMMAND sh -c "patch -p1 -l < ${PROJECT_BINARY_DIR}/libs-patches/zig_aarch64_grpc.patch"
        WORKING_DIRECTORY "${FALCOSECURITY_LIBS_SOURCE_DIR}")

if (USE_DYNAMIC_LIBELF)
    # Patch libelf.cmake to avoid using including /usr/include,
    # otherwise we would build with mixed system headers (from zig and from the host system).
    execute_process(
            COMMAND sh -c "patch -p1 -l < ${PROJECT_BINARY_DIR}/libs-patches/zig_libelf.patch"
            WORKING_DIRECTORY "${FALCOSECURITY_LIBS_SOURCE_DIR}")
endif()