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

  /**
   * Get user data.
   */
  public function getUser($id, $user_id) {
    $parameters = array(
      'instnr' => $id,
      'brugerid' => $user_id,
    );
    $res = $this->callWithAuth('hentInstBruger', $parameters, 'instBruger');

    return $res;
  }

  /**
   * Get main group of user.
   */
  public function getUserMainGroup($id, $user_id) {
    $user = $this->getUser($id, $user_id);
    if (isset($user->elev->hovedgruppenavn)) {
      return $user->elev->hovedgruppenavn;
    }

    return NULL;
  }

}
