#
# Copyright (c) 2018 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mqtt-simple)

# NORDIC SDK APP START
target_sources(app PRIVATE src/main.c)
target_include_directories(app PRIVATE ${ZEPHYR_BASE}/subsys/fs/nvs)
target_include_directories(app PRIVATE ${ZEPHYR_BASE}/subsys/net)
# NORDIC SDK APP END
