# ttyio


## Serial UART Quick and dirty bash script
## SPEED ia 115200 allways. CHnage the main.cpp otherwise

    - Ad: Buy some of my sdcards:  [R_PI PRELOADED SD CARDS](https://redypis.org/) 


Sample to sign in to over ttyUSB0


```bash
#!/bin/bash
echo "reset the board"

PROMPT="user@hostname:"

# wait 100 seconds for the Login: prompt
RET=$(./ttyio /dev/ttyUSB0 CRLF Login 100 GET) 

if [[ ${ERT} =~ "Login" ]];then
  # send the 'username' and wait for password prompt 5 sedconds
  RET=$(./ttyio /dev/ttyUSB0 username Password 5)
  if [[ ${ERT} =~ "Password" ]];then
    RET=(./ttyio /dev/ttyUSB0 'password' PROMPT 5 GET)
    #   
    [[ ${RET} =~ ${PROMPT} ]] && echo "signed in" && exit 0
  fi
fi
echo "failed to sing"
exit 1
```






```


