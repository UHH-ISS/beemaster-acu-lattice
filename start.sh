#!/bin/sh

docker build . -t acu
docker run acu
