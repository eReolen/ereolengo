/* global alert, confirm, prompt, XMLHttpRequest, ereolen_givadgang_api_url, ereolen_givadgang_api_token */
(function () {
  const getInstitutions = () => {
    const table = document.querySelector('table')
    const rows = [].slice.apply(table.querySelectorAll('tbody > tr'))

    const institutions = {}
    rows.forEach((row) => {
      const cell = row.querySelector('td:nth-child(2)')
      if (cell !== null) {
        const match = /^([a-z0-9]{6}) - (.+)/i.exec(cell.innerText)
        if (match !== null) {
          const id = match[1]
          const name = match[2]
          const group = row.querySelector('td:nth-child(3)').innerText
          const type = row.querySelector('td:nth-child(4)').innerText
          const numberOfMembers = parseInt(row.querySelector('td:nth-child(5)')?.innerText ?? 0)

          if (id in institutions) {
            if (!/\(\d+\)/.test(institutions[id].group)) {
              institutions[id].group += ` (${institutions[id].number_of_members})`
            }
            institutions[id].group += `; ${group} (${numberOfMembers})`
            institutions[id].type += `; ${type}`
            institutions[id].number_of_members += numberOfMembers
          } else {
            institutions[id] = {
              id,
              name,
              group,
              type,
              number_of_members: numberOfMembers
            }
          }
        }
      }
    })

    return institutions
  }

  /* eslint-disable camelcase */
  let apiUrl = typeof ereolen_givadgang_api_url !== 'undefined' ? ereolen_givadgang_api_url : 'https://ereolengo.dk'
  let apiToken = typeof ereolen_givadgang_api_token !== 'undefined' ? ereolen_givadgang_api_token : ''
  /* eslint-enable camelcase */

  if (!apiUrl) {
    apiUrl = prompt('apiUrl', apiUrl)
  }
  if (!apiToken) {
    apiToken = prompt('apiToken', apiToken)
  }
  if (!apiUrl) {
    alert('Missing API URL!')
    return
  }
  if (!apiToken) {
    alert('Missing API token!')
    return
  }

  try {
    const institutions = getInstitutions()

    if (Object.keys(institutions).length === 0) {
      alert('No institutions found!')
      return
    }

    if (confirm(`Export ${Object.keys(institutions).length} institutions to ${apiUrl}?`)) {
      const xhr = new XMLHttpRequest()
      xhr.open('POST', apiUrl)
      xhr.setRequestHeader('x-authorization', `token ${apiToken}`)
      xhr.setRequestHeader('content-type', 'application/vnd.api+json')
      xhr.send(JSON.stringify({ data: institutions }))

      xhr.onreadystatechange = function (event) {
        if (this.readyState === 4) {
          if (this.status === 204) {
            alert(`${Object.keys(institutions).length} institutions succesfully exported to ${apiUrl}`)
          } else {
            alert(`Error exporting ${Object.keys(institutions).length} institutions to ${apiUrl}:\n${xhr.status} ${xhr.response}`)
            console.log(xhr)
          }
        }
      }
    }
  } catch (error) {
    alert(error)
  }
}())
