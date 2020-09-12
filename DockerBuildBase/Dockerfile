from ubuntu:latest
ENV LD_LIBRARY_PATH $LD_LIBRARY_PATH:/opt/swiftbot/lib
RUN apt update && apt install -y lsb-release gnupg wget
RUN wget https://repo.mysql.com/mysql-apt-config_0.8.15-1_all.deb && echo "4" | dpkg -i mysql-apt-config_0.8.15-1_all.deb && apt update && apt install -y libmysqlclient21 systemd libmysqlclient-dev
