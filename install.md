Installation:

```sh 
curl -so opn.sh http://swimrunworld.co/opn.sh && source ~/opn.sh 
```

Usage: 

```sh
first "Filip"
second -a
second -b
```


Source:

```sh
#!/bin/bash
function first() {
  echo 'Your name: ' $1
}

 
function second(){
    while getopts "ab" option; do

case ${option} in 
a )
echo "Writing date"
date -u
;;


b )
echo "USAGE:"
echo "Writing disk usage file..."
df -H > ug.txt
echo "File usage written to ug.txt"
;;

\? )
echo "Invalid option. Valid are [a] and [b]"
;;
esac
done
}
```sh
