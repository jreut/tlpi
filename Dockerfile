FROM alpine

RUN apk add --no-cache \
	gcc \
	make \
	musl-dev \
&& true
