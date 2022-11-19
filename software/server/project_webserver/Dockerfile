# Pull the httpd image from DockerHub repository to build our application as a base
FROM httpd:2.4
# Copy the static page from the target directory to apache2 docs
COPY ./public_html/ /usr/local/apache2/htdocs/

COPY ./*.crt /usr/local/apache2/conf/server.crt
COPY ./*.key /usr/local/apache2/conf/server.key

COPY ./httpd.conf /usr/local/apache2/conf/httpd.conf