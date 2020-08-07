#!/bin/sh
kill `ps -ef|grep -v grep |grep team| awk '{print $2}'`
kill `ps -ef|grep -v grep |grep broker| awk '{print $2}'`
kill `ps -ef|grep -v grep |grep gameboy| awk '{print $2}'`
kill `ps -ef|grep -v grep |grep gamecard| awk '{print $2}'`
kill `ps -ef|grep -v grep |grep valgrind| awk '{print $2}'`
