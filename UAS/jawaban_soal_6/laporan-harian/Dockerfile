FROM php:7.4-fpm

RUN apt-get update && apt-get install -y \
    git \
    unzip

RUN docker-php-ext-install pdo pdo_mysql

# Install Composer
COPY --from=composer:latest /usr/bin/composer /usr/bin/composer

WORKDIR /var/www

COPY . .

# Install Composer dependencies
RUN composer install

RUN chown -R www-data:www-data /var/www

EXPOSE 9000
CMD ["php-fpm"]
