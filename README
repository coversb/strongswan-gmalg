# strongswan with GM #

## Overview ##
1. 基于开源的strongswan，以plugin的方式添加国密算法，实现国密加密的IPSec VPN
2. SM3/SM4已验证可以作为IPSec VPN的加密方式使用;SM2目前无需求，未验证
3. strongswan支持使用TUN设备的应用层IPSec功能和基于内核xfrm的IPSec功能，由于内核xfrm需要内核加密支持，在网上找到的国密内核驱动可以编译出KO文件并加载至内核，但是未能正常工作，目前验证通过是使用TUN设备方式实现（通过configure时 --enable-kernel-libipsec开启）

## Compile & Install ##
1 ./autogen.sh

2 ./configure --enable-gmalg --enable-kernel-libipsec --prefix=XXXX(Install PATH)

3 make

4 make install

