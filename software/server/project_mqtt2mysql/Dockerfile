# Build Application Container
FROM alpine:3

RUN apk add -U --no-cache python3 && \
    apk add -U --no-cache python3-dev && \
    apk add -U --no-cache py3-pip && \
    addgroup -g 1000 app && \
    adduser -D -u 1000 -G app app && \
    mkdir /app

# Install Application
COPY app/* /app/
WORKDIR /app
RUN chown -R app:app /app && \
    pip3 install -r requirements.txt --upgrade pip

# Image Settings
USER app
ENTRYPOINT ["python3"]
CMD ["mqtt2mysql.py"]
