import React from 'react'

const Municipality = ({ municipality, institutions }) => (
  <tbody>
    <tr className='municipality'>
      <td className='municipality-name'>{municipality.kommune}</td>
      <td colSpan={3} />
    </tr>
    {institutions.map(institution =>
      <tr key={'i' + institution.id}>
        <td />
        <td className='institution-name'>{institution.name}</td>
        <td className='institution-id'>{institution.id}</td>
        <td className='institution-number-of-members'>{institution.number_of_members}</td>
      </tr>
    )}
  </tbody>
)

export default Municipality
