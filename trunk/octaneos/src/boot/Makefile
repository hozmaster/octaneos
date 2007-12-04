#
# Copyright (C) 2003, 2007 Berlin Brown
# boot - Makefile
#
# Copyright (C) 2007 Berlin Brown
# Octane OS (Operating System)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

all:

.PHONY: bootsect boot

#
# Clean up the garbage including GCC generated ASM files *.s
#
clean:
	@echo " cleaning : boot"
	rm -f bootsect
	rm -f setup setup.s mini
	rm -f vmlinux _tmp_*
	rm -f *.o *.s *~