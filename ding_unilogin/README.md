# Ding Unilogin

## API

Go to `/admin/config/ding/unilogin_api` to set the api token.

### Endpoints

This api adheres to the [JSON:API specification](https://jsonapi.org/).

| Request                  | Description                         |
|--------------------------|-------------------------------------|
| `GET /institutions`      | Get list of all institutions        |
| `GET /institutions/«id»` | Get details on a single institution |
| `POST /institutions`     | Update the list of institutions     |

#### Examples

```sh
curl http://127.0.0.1/unilogin/api/institutions \
     --header 'x-authorization: token «api read token»
```

**Important**: Note that the header name is `x-authorization` (not
`authorization`)!

```sh
curl http://127.0.0.1/unilogin/api/institutions/«id» \
     --header 'x-authorization: token «api read token»
```

## Tests

Note: The tests are run against the current site and will change the list of
institutions!

```sh
itkdev-docker-compose drush --yes pm-enable simpletest
itkdev-docker-compose drush --uri=http://nginx0 test-run ding_unilogin
```
