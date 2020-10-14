# MySQL server configuration

### Connect to your server

Use root password from your database

```shell script
mysql -uroot
```

### Create database and user with required privileges

```mysql
create database swiftbot;
use mysql; 
CREATE USER 'swiftbot'@'%' IDENTIFIED BY 'swiftbot'; 
GRANT ALL ON *.* TO 'swiftbot'@'%'; 
flush privileges;
```

### Import release dump with required structure to you database
```shell script
mysql -u mysql_user -p mysqlpassword createddbname < /opt/swiftbot/swiftbot.sql
```

## SWIFT.bot - [Back to home](HomePage.md)