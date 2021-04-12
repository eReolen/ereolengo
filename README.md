# ereolengo.dk

Modules for ereolengo.dk.

## Coding standards

All code must adhere to [the Drupal Coding
Standards](https://www.drupal.org/docs/develop/standards).

Checking the coding standards automatically is a bit tricky as two different PHP
versions are needed (!): PHP 7.2 is needed to install the tools; PHP 7.0 is
needed to check the code to prevent false negatives.

```sh
# Install composer packages using PHP 7.2
docker run --rm -it --volume $(pwd):/app itkdev/php7.2-fpm:latest composer install
```

Check the coding standards:

```sh
# The code is run using PHP 7.0 se we need to check it using PHP 7.0
docker run --rm -it --volume $(pwd):/app itkdev/php7.0-fpm:latest composer coding-standards-check
```

Apply the coding standards:

```sh
# The code is run using PHP 7.0 se we need to apply it using PHP 7.0
docker run --rm -it --volume $(pwd):/app itkdev/php7.0-fpm:latest composer coding-standards-apply
```
