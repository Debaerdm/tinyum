#/bin/bash

prog=tinyum
path=$1
des=$2
mail=$3
year=$(date +'%Y')
fullname=$(getent passwd $USER | cut -d ':' -f 5)
copyright="Copyright (C) $year $fullname $mail"

header="
*
* $path
* $des
*
* $copyright 
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
* NON INFRINGEMENT.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
"

if [[ -e $1 ]]; then
		tmp=$(cat $1 | grep "Copyright" | wc -l)
		line=$(cat $1 | grep -n "Copyright" | head -n 1 | cut -d ':' -f 1)
		echo `expr $line + $tmp`
		#sed -i '$calul\ 
		#$copyright\
		#			' $1
else
		touch $1 ; echo "/$header/" | cat - $1 > /tmp/temp && mv /tmp/temp $1
fi


#| cat - $1 > /tmp/temp && mv /tmp/temp $1


