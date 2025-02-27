sid_cfg = "SID_RT_ELIGIBLE=1,SID_URGENCY_LEVEL=2"
rp_cfg = "RP_RT_NOTIONAL=0,RP_RO_NOTIONAL=0,RP_RO_PAUSED=0,RP_RO_ELIGIBLE=1"
gbl_cfg = "GBL_MAX_ORD_QTY=10000000000,GBL_MAX_NOTIONAL=9999000000000,GBL_CO_WAIT=300"
sym_cfg = "SYM_RO2RT_MIN=15,SYM_RO2RT_MAX=30,SYM_MRI_FIXED_WAIT=250,SYM_MRI_RANDOM_WAIT=50,SYM_PRF_WAIT=5000000"
cindex = -1 
findex = -1
keyid = 999999
sindex = -1

def config(cfg, symidx = 0, sym = "", useridx = 0, user = "0"):
    cfg = cfg.split("=")
    global cindex
    cindex = 1+cindex
    print("DomainObject=ConfigLookup,fastindex=" + str(cindex) +",ConfigId=" + str(cindex) + \
        ",ConfigName="+ cfg[0] + ",SymIdx=" + str(symidx) + ",Symbol=" + sym + \
        ",FirmIdx=" + str(useridx) + ",FirmId=" + user + ",ConfigValue=" + cfg[1] + ",ConfigScale=0")

def symbol(sym):
    global sindex 
    sindex = sindex+1
    print("DomainObject=SymbolLookup,fastindex=" + str(sindex) + ",Name="+ sym + ",SymbolStatus=OPEN" + \
        ",TransactionNumber=0,NBBOBidPx=0,NBBOAskPx=0,INBidPx=0,INAskPx=0,RPBidPx=0,RPAskPx=0")
    for cfg in sym_cfg.split(","):
        config(cfg, symidx=sindex, sym=sym)

def firm(name, type, rpids = ",,", mpids = ","):
    global findex, keyid
    findex = findex+1
    keyid = keyid+1
    print("DomainObject=FirmLookup,fastindex=" + str(findex) + \
            ",FirmId=" + str(keyid) +",Name="+ name + \
            ",FirmRecordType="+type+",ParentFirm=0,ParentMPID=0,ClientID=0")
    firmid = keyid 
    if type == "MATCHER" or type == "GATEWAY": 
        return
    arr = "PLATINUM,GOLD,SILVER".split(",")
    for nam, typ in zip(rpids.split(","), arr):
        if nam == "": continue
        findex = findex+1
        keyid = keyid+1
        print("DomainObject=FirmLookup,fastindex=" + str(findex) + \
                ",FirmId=" + str(keyid) +",Name="+ nam + \
                ",FirmRecordType=RP_"+typ+",ParentFirm="+str(firmid) + \
                ",ParentMPID=0,ClientID=0")
        for cfg in rp_cfg.split(","):
            config(cfg, useridx=findex, user=str(keyid))
    mpids = mpids.split(",")
    if mpids[0] == "": return
    findex = findex+1
    keyid = keyid+1
    print("DomainObject=FirmLookup,fastindex=" + str(findex) + \
            ",FirmId=" + str(keyid) +",Name="+ mpids[0] + \
            ",FirmRecordType=MPID,ParentFirm="+str(firmid) + \
            ",ParentMPID=0,ClientID=0")
    mpid = keyid 
    for nam in mpids[1:]:
        if nam == "": continue
        findex = findex+1
        keyid = keyid+1
        print("DomainObject=FirmLookup,fastindex=" + str(findex) + \
                ",FirmId=" + str(keyid) +",Name="+ nam + \
                ",FirmRecordType=SUBID,ParentFirm="+str(firmid) + \
                ",ParentMPID="+str(mpid)+",ClientID=0")
        for cfg in sid_cfg.split(","):
            config(cfg, useridx=findex, user=str(keyid))

for cfg in gbl_cfg.split(","):
    config(cfg)
for sym in "NVDA GOOG MSFT".split():
    symbol(sym)
firm("GWY01", "GATEWAY")
firm("ME01", "MATCHER")
firm("GOLDMAN", "FIRMID", "GSRPP,GSRPG,GSRPS", "GLD01,GLDA,GLDB")
