<?php

/**
 * @file
 * Class for communicating with the WS03 service at Unilogin.
 *
 * @see https://viden.stil.dk/pages/viewpage.action?pageId=2360660
 */

/**
 * Class for the license service.
 */
class DingUniloginWSALicens extends DingUniloginServiceBase {
  const WSDLURL = 'https://wsalicens.uni-login.dk/wsalicens-v4/ws?WSDL';

  /**
   * Hent grupper med licens.
   */
  public function hentGrupperMedLicens($udbydernr, $tjenestekode, $instnr = NULL) {
    return $this->callWithAuth('hentGrupperMedLicens', array(
      'udbydernr' => $udbydernr,
      'tjenestekode' => $tjenestekode,
      'instnr' => $instnr,
    ));
  }

}
