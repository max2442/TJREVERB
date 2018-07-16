import logging
from . import aprs

def dispatch_command(packet):
    logging.debug("DISPATCH CALLED")
    rawpacket = str(packet)
    logging.info("FROM APRS: "+rawpacket)
    headerfindresult = rawpacket.find(':')
    if headerfindresult == -1:
        logging.info("INCOMPLETE HEADER")
        return
    header = rawpacket[:headerfindresult]
    logging.info("HEADER: "+header)
    datacontent = rawpacket[headerfindresult+1:]

    if len(datacontent) == 0:
        logging.info("EMPTY BODY")
        return

    logging.info("BODY: "+datacontent)
    decode(datacontent)
def checksum(body):
    sum = sum([ord(x) for x in body[0:-6]])
    sum %= 256
    return sum == body[-6]


def decode(body):
    logging.debug(body[-5:-1])
    logging.debug(body[0:2])
    if body[0:2] == 'TJ' and body[-5:-1] == '\\r\\n' and checksum(body):

        logging.debug('VALID MESSAGE')
        modules[body[2]][body[3]](body[4])
    else:
        logging.debug('INVALID MESSAGE')
def piprint(packet):
    print("FROM APRS: "+str(packet))

def on_startup():
    global modules, m_aprs
    #modules = {'A':core,'B':m_aprs,'C':'iridium','D':'housekeeping','E':'log','F':'GPS'}
    m_aprs = {'a':aprs.send}
    modules = {'B':m_aprs}
    #core = {}
