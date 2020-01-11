# Ding Unilogin

## API

Go to `/admin/config/ding/unilogin_api` to set the api token.

https://jsonapi.org/

### Endpoints

| Request                  | Description                         |
|--------------------------|-------------------------------------|
| `GET /institutions`      | Get list of all institutions        |
| `GET /institutions/«id»` | Get details on a single institution |
| `POST /institutions`     | Update the list of institutions     |

## Tests

Note: The tests are run against the current site and will change the list of
institutions!

```sh
itkdev-docker-compose drush --uri=http://nginx0 test-run ding_unilogin
```
