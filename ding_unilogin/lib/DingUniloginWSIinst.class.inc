<?php

/**
 * @file
 * Class for communicating with the ws22/wsiINST service at Unilogin.
 */

/**
 * Class for the instiution information service.
 */
class DingUniloginWSIinst extends DingUniloginServiceBase {
  const WSDLURL = 'https://wsiinst.unilogin.dk/wsiinst-v5/ws?WSDL';

  /**
   * Get institution.
   *
   * @return object
   *   Institution data.
   */
  public function getInstitution($id) {
    $parameters = array(
      'instnr' => $id,
    );
    $res = $this->callWithAuth('hentInstitution', $parameters, 'institution');

    return $res;
  }

  /**
   * Get institutions.
   *
   * @param array $ids
   *   The list of institution ids.
   *
   * @return object[]
   *   List of institution data.
   */
  public function getInstitutions(array $ids) {
    $parameters = array(
      'instnr' => $ids,
    );
    $res = $this->callWithAuth('hentInstitutioner', $parameters, 'institution');

    return $res;
  }

}
