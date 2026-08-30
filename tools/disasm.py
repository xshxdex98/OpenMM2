import sys, json, capstone
sys.path.insert(0,'tools')
from pe import IMAGE_BASE
from verify_data import sections
RETAIL="C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe"
_,S=sections(RETAIL)
SYM={x['rva']+IMAGE_BASE:x['mangled'] for x in json.load(open('data/symbols.json',encoding='utf-8'))}
def sec_for(va):
    for n,s in S.items():
        b=s['va']+IMAGE_BASE
        if b<=va<b+len(s['bytes']): return s['bytes'], b
    return None,0
def dis(va, n=40):
    data,base=sec_for(va)
    if data is None: print('bad va'); return
    off=va-base
    md=capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail=False
    out=[]
    for i in md.disasm(bytes(data[off:off+n*8]), va):
        t=SYM.get(i.address)
        if t and i.address!=va: out.append('  %s:'%t)
        c=''
        # annotate call/jmp targets
        op=i.op_str
        if op.startswith('0x'):
            try:
                tgt=int(op,16)
                if tgt in SYM: c='   ; '+SYM[tgt][:60]
            except: pass
        for tok in op.replace('[',' ').replace(']',' ').split():
            tok=tok.strip(',')
            if tok.startswith('0x'):
                try:
                    v=int(tok,16)
                    if v in SYM and not c: c='   ; '+SYM[v][:60]
                except: pass
        out.append('  %08X  %-9s %s%s'%(i.address,i.mnemonic,op,c))
        if i.mnemonic in ('ret','retn') : break
        if len(out)>=n: break
    print('\n'.join(out))
for name,va in [(a,int(b,16)) for a,b in (x.split('=') for x in sys.argv[1:])]:
    print('==== %s @ %08X ===='%(name,va)); dis(va, int(__import__("os").environ.get("N","46"))); print()
