import './list.scss'

import React, { useEffect, useState } from 'react'
import ReactDOM from 'react-dom'
import Municipality from './components/Municipality'
const config = window.Drupal.settings.ding_unilogin

const App = ({ api_url: apiUrl }) => {
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState(null)
  const [data, setData] = useState([])
  const [search, setSearch] = useState('')
  const [filteredData, setFilteredData] = useState([])

  const fetchOptions = { headers: { accept: 'application/json' } }

  useEffect(() => {
    window.fetch(apiUrl, fetchOptions)
      .then((response) => response.json())
      .then((institutions) => {
        // Group institutions by municipality.
        const municipalities = {}
        for (const { municipality, ...institution } of Object.values(institutions.data)) {
          if (!municipalities[municipality.kommunenr]) {
            municipalities[municipality.kommunenr] = {
              municipality: municipality,
              institutions: []
            }
          }
          municipalities[municipality.kommunenr].institutions.push(institution)
        }

        // Sort institutions by name.
        for (const { institutions } of Object.values(municipalities)) {
          institutions.sort((i1, i2) => i1.name.localeCompare(i2.name))
        }

        // Sort municipalities by name.
        const data = Object.values(municipalities).sort((m1, m2) => m1.municipality.kommune.localeCompare(m2.municipality.kommune))

        setData(data)
        setLoading(false)
      }).catch((err) => {
        setError(err)
        setLoading(false)
      })
  }, [])

  useEffect(() => {
    const query = search.toLocaleLowerCase()
    const filtered = data
    // Filter institutions on name and id
      .map(item => ({
        ...item,
        institutions: item.institutions.filter(institution =>
          institution.name.toLocaleLowerCase().indexOf(query) > -1 ||
            institution.id.toLocaleLowerCase().indexOf(query) > -1
        )
      }))
    // Filter out municipalities with no institutions
      .filter(({ institutions }) => institutions.length > 0)

    setFilteredData(filtered)
  }, [data, search])

  return (
    <>
      {!loading && !error && (
        <>
          <input value={search} onChange={(e) => setSearch(e.currentTarget.value)} placeholder='Søg i skolenavn eller institutionsnummer' />
          <table className='institution-list'>
            <thead>
              <tr>
                <th>Kommune</th>
                <th>Skolenavn</th>
                <th>Institutionsnummer</th>
                <th>Antal elever</th>
              </tr>
            </thead>
            {(filteredData && filteredData.length > 0)
              ? filteredData.map(item => <Municipality key={'m' + item.municipality.kommunenr} {...item} />)
              : <tbody><tr><td colSpan='4'>Ingen institutioner fundet</td></tr></tbody>}
          </table>
        </>
      )}
      {loading && <div className='loading'>Henter institutioner …</div>}
      {error && <div className='error'>{error}</div>}
    </>
  )
}

ReactDOM.render(
  <React.StrictMode>
    <App {...config} />
  </React.StrictMode>,
  document.getElementById('app')
)
