# ttyio


## Serial UART Quick and dirty bash script
## SPEED ia 115200 allways. CHnage the main.cpp otherwise

[R_PI PRELOADED SD CARDS](https://redypis.org/) 


Sample to sign in to over ttyUSB0


```bash
#!/bin/bash


PROMPT="user@hostname:"

echo "reset the board you're wired to over ttyUSB# or ttyACM#"
# wait 100 seconds for the Login: prompt
RET=$(./ttyio /dev/ttyUSB0 CRLF Login 100 GET) 

if [[ ${RET} =~ "Login" ]];then

  # send the 'username' and wait for password prompt 5 seconds
  RET=$(./ttyio /dev/ttyUSB0 username Password 5)
  if [[ ${ERT} =~ "Password" ]];then
    RET=(./ttyio /dev/ttyUSB0 'password' PROMPT 5 GET)
    [[ ${RET} =~ ${PROMPT} ]] && echo "signed in" && exit 0
  fi
fi
echo "failed to sing in"
exit 1
```

####  Usage

ttyio  device send_command  expect_string timeout <REP times, interval> <GET> <DEBUG> <SEND>
  * send command
    * "any string"
    * UP sends up key
    * CR sends \r
    * LF sends \n
    * CRLF sends \r\n
  * expect string  any string. * expectes forever or timeout.
    * when found print;s the string and exits with 0
  * timeout to expect in seconds. expires return code !=0.   
  * <REP TIMES INTER>   optional, repeats to send the command TIMES at interval INTER when expected output is found
  * GET when prgogram exists (success or not) prints all recevied buffer
  * DEBUG  debug on. prints between {} what sends and what receives
  * SEND sends only and exits
  



