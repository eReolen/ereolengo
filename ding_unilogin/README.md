# Ding Unilogin

## API

Go to `/admin/config/ding/unilogin_api` to set the api token.

### Endpoints

This api adheres to the [JSON:API specification](https://jsonapi.org/).

| Request                               | Description                         |
|---------------------------------------|-------------------------------------|
| `GET /unilogin/api/institutions`      | Get list of all institutions        |
| `GET /unilogin/api/institutions/«id»` | Get details on a single institution |
| `POST /unilogin/api/institutions`     | Update the list of institutions     |

#### Examples

```sh
curl http://127.0.0.1/unilogin/api/institutions \
     --header 'x-authorization: token «api read token»'
```

**Important**: Note that the header name is `x-authorization` (not
`authorization`)!

```sh
curl http://127.0.0.1/unilogin/api/institutions/«id» \
     --header 'x-authorization: token «api read token»'
```

## Tests

Note: The tests are run against the current site and will change the list of
institutions!

```sh
docker-compose run --rm drush --root=/app --yes pm-enable simpletest
docker-compose run --rm drush --root=/app --uri=http://nginx0 test-run ding_unilogin
```
