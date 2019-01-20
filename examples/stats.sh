#!/bin/sh
#
#
#

ps aux | grep 'pair'  | grep -v grep | grep -v cloud | awk '{print $2}' | parallel kill -14 {}
