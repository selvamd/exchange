#!/bin/sh
ps -ef | grep svc | awk  '{print $2}' | xargs kill 
sleep 2
cd ../bin
./loginsvc & 
./accountsvc & 
./gatewaysvc & 
#say started
