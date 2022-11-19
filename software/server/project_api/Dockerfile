# Build Application Container
FROM alpine:3

RUN apk add -U --no-cache python3 && \
    apk add -U --no-cache python3-dev && \
    apk add -U --no-cache py3-pip && \
    apk add -U --no-cache mariadb-connector-c && \
    apk add -U --no-cache mariadb-connector-c-dev && \
    apk add -U --no-cache alpine-sdk && \
    addgroup -g 1000 app && \
    adduser -D -u 1000 -G app app && \
    mkdir /flask_api

# Install Application
COPY flask_api/* /flask_api/
WORKDIR /flask_api
RUN chown -R app:app /flask_api && \
    pip3 install -r requirements.txt --upgrade pip

# Image Settings
USER app
ENTRYPOINT ["python3"]
CMD ["main.py"]