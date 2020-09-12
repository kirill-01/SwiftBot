from swiftbot-core-base:latest
ENV LD_LIBRARY_PATH $LD_LIBRARY_PATH:/opt/swiftbot/lib
COPY core.deb /tmp/app.deb 
RUN dpkg -i /tmp/app.deb && rm -rf core.deb
