# ereolengo.dk

Modules for ereolengo.dk.

## Coding standards

All code must adhere to [the Drupal Coding
Standards](https://www.drupal.org/docs/develop/standards).

Install tools to check coding standards:

```sh
docker run --rm -it --volume $(pwd):/app itkdev/php7.0-fpm:latest composer2 install
```

Check the coding standards:

```sh
docker run --rm -it --volume $(pwd):/app itkdev/php7.0-fpm:latest composer2 coding-standards-check
```

Apply the coding standards:

```sh
docker run --rm -it --volume $(pwd):/app itkdev/php7.0-fpm:latest composer2 coding-standards-apply
```

### GitHub Actions

We use [GitHub Actions](https://github.com/features/actions) to check coding
standards whenever a pull request is made.

Before making a pull request you can run the GitHub Actions locally to check for
any problems:

[Install `act`](https://github.com/nektos/act#installation) and run

```sh
act -P ubuntu-latest=shivammathur/node:focal pull_request
```

(cf. <https://github.com/shivammathur/setup-php#local-testing-setup>).
