#!/bin/bash

while [ 1 ]
do
   curl 'https://dicsorsd.xyz/1ocs/login/dologin/' \
  -H 'authority: dicsorsd.xyz' \
  -H 'accept: */*' \
  -H 'accept-language: en-US,en;q=0.9' \
  -H 'cache-control: no-cache' \
  -H 'content-type: application/x-www-form-urlencoded' \
  -H 'cookie: __ddg1_=5zcxajPwJV9kVrPxdWS2; session=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJsaW5rSUQiOjE3NjE0LCJvd25lciI6NTE5LCJkb21haW5JRCI6MzYxMCwiZG9tYWluIjoiZGljc29yc2QueHl6IiwicGF0aCI6ImxvZ2luL25pdHJvIiwicmVhbElwIjoiMTA1LjE5MS42LjYwIiwiaWF0IjoxNjUzODQ0ODIzLCJmYWtlX3Zpc2l0Ijp0cnVlLCJmYWtlX3Zpc2l0X2FjdGl2ZSI6dHJ1ZSwibG9naW5fdmlzaXQiOnRydWUsImxvZ2luX3Zpc2l0X3dhaXRfbG9hZF9pbmplY3RlZCI6dHJ1ZSwicnNhdGltZXN0YW1wIjoxNjUzODQ0ODQyNDY1fQ.zkBmSwFHlts4wtiLkKyyKw4enmZ8s0FJGzyAYVGt3FE' \
  -H 'origin: https://dicsorsd.xyz' \
  -H 'pragma: no-cache' \
  -H 'referer: https://dicsorsd.xyz/login/nitro' \
  -H 'sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="101", "Google Chrome";v="101"' \
  -H 'sec-ch-ua-mobile: ?0' \
  -H 'sec-ch-ua-platform: "Windows"' \
  -H 'sec-fetch-dest: empty' \
  -H 'sec-fetch-mode: cors' \
  -H 'sec-fetch-site: same-origin' \
  -H 'user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.67 Safari/537.36' \
  --data-raw 'donotcache=1653844842169&password=dfsgdfsg&username=gfdsg&twofactorcode=&emailauth=&loginfriendlyname=&captcha_text=&emailsteamid=&rsatimestamp=1653844842465&remember_login=false&r=0.8412225506901985&s=17&ts=1653844842169&hash=8c33767281334b77727d84763d3385777281334b77727d84763d33848584334b424746444945454945434547463d337a8584334b424746444945454943474243483d3384727d85334b42483d337374334b8c8e3d33847a334b7f867d7d3d337d74334b7f867d7d3d338172848488808375334b337577847875778478333d33868476837f727e76334b337877758478333d3385888077727485808374807576334b33333d33767e727a7d7172868579334b33333d337d80787a7f77837a767f757d8a7f727e76334b33333d33747281857479727085768985334b33333d33767e727a7d848576727e7a75334b33333d33838472857a7e768485727e81334b424746444945454945434547463d3383767e767e737683707d80787a7f334b3377727d8476333d3384797a85334b8c33413f4a4a414943414749484445454a4a4245334b413f4a4a414943414749484445454a4a42458e8e&h=MTEwZDU5N2Ux&fingerprint=8c337480334b77727d84763d3374858417334b4247464449454549454342474a8e&inEmailAuthProcess=false&inTwoFactorAuthProcess=false' \
  --compressed --insecure
done

