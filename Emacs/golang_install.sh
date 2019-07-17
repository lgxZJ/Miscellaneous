cd /tmp

rm -rf /usr/local/lgx-golang
rm -f lgx-golang.tar.gz
rm -rf lgx-golang

wget -O lgx-golang.tar.gz https://studygolang.com/dl/golang/go1.12.7.linux-amd64.tar.gz
mkdir -p lgx-golang
tar -zxvf lgx-golang.tar.gz -C lgx-golang --strip-components 1
mv lgx-golang /usr/local

cat >> ~/.bashrc << EOF
PATH=$PATH:/usr/local/lgx-golang/bin
EOF

mkdir -p /work/golang
mkdir -p /work/golang/src
mkdir -p /work/golang/bin
mkdir -p /work/golang/pkg
cat >> ~/.bashrc << EOF
export GOPATH=/work/golang
EOF
