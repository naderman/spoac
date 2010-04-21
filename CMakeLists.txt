# This file is part of SPOAC.
# Copyright (c) 2004-2010 GearBox team
# Copyright (c) 2004-2010 Orca Robotics team
# Copyright (c) 2010 Nils Adermann (modifications)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

cmake_minimum_required(VERSION 2.6.3 FATAL_ERROR)

# project name
project(spoac)
set(SPOAC_PROJECT_NAME_CAP "Spoac")
set(SPOAC_PROJECT_NAME_LOWER "spoac")

# project version
set(SPOAC_PROJECT_VERSION_MAJOR "0")
set(SPOAC_PROJECT_VERSION_MINOR "1")
set(SPOAC_PROJECT_VERSION_MAINTENANCE "0")

set(SPOAC_PROJECT_VERSION ${SPOAC_PROJECT_VERSION_MAJOR}.${SPOAC_PROJECT_VERSION_MINOR}.${SPOAC_PROJECT_VERSION_MAINTENANCE})

# Set up everything else
include(cmake/Setup.cmake)
