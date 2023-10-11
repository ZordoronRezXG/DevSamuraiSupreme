@echo off

set /p commit_message=<commitmsg.txt
git add .
git commit -m "%commit_message%"
git push
