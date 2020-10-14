# NGINX configuration

## You need SSL certificates on host system.

If you need to create new one, proceed current instruction. Otherwise, skip this step.

## Default configuration

Modify **/etc/nginx/default.conf**

(Replace example.com with your domain name)

```editorconfig
server {
    listen 443;
    # host name to respond to
    server_name example.com;
    # your SSL configuration
    ssl on;
    ssl_certificate /etc/letsencrypt/live/example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/example.com/privkey.pem;

    location /ws {
        # switch off logging
        access_log off;
        # redirect all HTTP traffic to localhost:8080
        proxy_pass http://127.0.0.1:8080;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header Host $host;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

        # WebSocket support (nginx 1.4)
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
    }
}
```

## Restart server

```shell script
systemctl resstart nginx
```

## SWIFT.bot - [Back to home](HomePage.md)