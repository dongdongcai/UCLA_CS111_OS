class Inode:
    def __init__(self, inodenumber):
        self.inodenumber = inodenumber
        self.referlist = []
        self.linkcount = 0
        self.pointers = []

class Block:
    def __init__(self, blocknumber):
        self.blocknumber = blocknumber
        self.referlist = []

def traverse(total, counter, indiraddress, level):
    if total == counter:
        return total
    if level == 0:
        if indiraddress == 0 or indiraddress > totalblocks:
            ans.write('INVALID BLOCK < ' + str(indiraddress) + ' > IN INODE < ' + str(tempinode.inodenumber) + ' >  ENTRY < ' + str(forlevel0) + ' >\n')
        else:
            if indiraddress not in allocatedblocks:
                tempblock = Block(indiraddress)
                tempdic = {}
                tempdic['inode'] = tempinode.inodenumber
                tempdic['indirect_block_num'] = -1
                tempdic['entry'] = forlevel0
                tempblock.referlist.append(tempdic)
                allocatedblocks[indiraddress] = tempblock
            else:
                tempdic = {}
                tempdic['inode'] = tempinode.inodenumber
                tempdic['indirect_block_num'] = -1
                tempdic['entry'] = forlevel0
                allocatedblocks[indiraddress].referlist.append(tempdic)                
        return counter + 1        
    elif level == 1:
        for i in range(0, min(blocksize / 4, total - counter)):
            if forlookup[(indiraddress, i)] == 0 or forlookup[(indiraddress, i)] > totalblocks:
                ans.write('INVALID BLOCK < ' + str(forlookup[(indiraddress, i)]) + ' > IN INODE < ' + str(tempinode.inodenumber) + ' >  INDIRECT BLOCK < ' + str(indiraddress) + ' > ENTRY < ' + str(i) + ' >\n')
            else:
                if forlookup[(indiraddress, i)] not in allocatedblocks:
                    tempblock = Block(forlookup[(indiraddress, i)])
                    tempdic = {}
                    tempdic['inode'] = tempinode.inodenumber
                    tempdic['indirect_block_num'] = indiraddress
                    tempdic['entry'] = i
                    tempblock.referlist.append(tempdic)
                    allocatedblocks[forlookup[(indiraddress, i)]] = tempblock
                else:
                    tempdic = {}
                    tempdic['inode'] = tempinode.inodenumber
                    tempdic['indirect_block_num'] = indiraddress
                    tempdic['entry'] = i
                    allocatedblocks[forlookup[(indiraddress, i)]].referlist.append(tempdic) 
        return i + counter + 1
    else:
        i = 0
        while (indiraddress, i) in forlookup and counter < total:
            if forlookup[(indiraddress, i)] == 0 or forlookup[(indiraddress, i)] > totalblocks:
                ans.write('INVALID BLOCK < ' + str(forlookup[(indiraddress, i)]) + ' > IN INODE < ' + str(tempinode.inodenumber) + ' >  INDIRECT BLOCK < ' + str(indiraddress) + ' > ENTRY < ' + str(i) + ' >\n')
            else:
                if forlookup[(indiraddress, i)] not in allocatedblocks:
                    tempblock = Block(forlookup[(indiraddress, i)])
                    tempdic = {}
                    tempdic['inode'] = tempinode.inodenumber
                    tempdic['indirect_block_num'] = indiraddress
                    tempdic['entry'] = i
                    tempblock.referlist.append(tempdic)
                    allocatedblocks[forlookup[(indiraddress, i)]] = tempblock
                else:
                    tempdic = {}
                    tempdic['inode'] = tempinode.inodenumber
                    tempdic['indirect_block_num'] = indiraddress
                    tempdic['entry'] = i
                    allocatedblocks[forlookup[(indiraddress, i)]].referlist.append(tempdic) 
            counter += 1
            counter = traverse(total, counter, forlookup[(indiraddress, i)], level - 1)            
            i += 1
        return counter


ans = open('lab3b_check.txt', 'w+')
allocatedblocks = {}

with open('super.csv', 'r') as superfile:
    for line in superfile:
        superentry = line.rstrip('\n').split(',')
        totalinodes = int(superentry[1])
        totalblocks = int(superentry[2])
        blocksize = int(superentry[3])
        inodespergroup = int(superentry[-3])
        firstdatablock = int(superentry[-1])
superfile.close()
allocatedblocks[firstdatablock] = Block(firstdatablock)
allocatedblocks[firstdatablock + 1] = Block(firstdatablock + 1)

blockfreemappos = []
inodefreemappos = []
with open('group.csv', 'r') as group:
    for line in group:
        groupentry = line.rstrip('\n').split(',')
        blockfreemappos.append(int(groupentry[-2], 16))
        inodefreemappos.append(int(groupentry[-3], 16))
        allocatedblocks[int(groupentry[-2], 16)] = Block(int(groupentry[-2], 16))
        allocatedblocks[int(groupentry[-3], 16)] = Block(int(groupentry[-3], 16))
        inodestart = int(groupentry[-1], 16)
        for i in range(0, inodespergroup / (blocksize / 128)):
            allocatedblocks[inodestart + i] = Block(inodestart + i)
        #allocatedblocks[int(groupentry[-1], 16)] = Block(int(groupentry[-1], 16))
group.close()

blockfreemap = set([])
inodefreemap = set([])
with open('bitmap.csv', 'r') as bitmap:
    for line in bitmap:
        freeentry = line.rstrip('\n').split(',')
        if int(freeentry[0], 16) in blockfreemappos:
            blockfreemap.add(int(freeentry[1]))
        else:
            inodefreemap.add(int(freeentry[1]))
bitmap.close()

forlookup = {}
with open('indirect.csv', 'r') as indirectfile:
    for line in indirectfile:
        indirectentry = line.rstrip('\n').split(',')
        # tempblock = Block(int(indirectentry[2], 16))
        # tempdic = {}
        # tempdic['indirect_block_num'] = int(indirectentry[0], 16)
        # tempdic['entrynum'] = int(indirectentry[1])
        # tempblock.referlist.append(tempdic)
        # allocatedblocks[int(indirectentry[2], 16)] = tempblock
        forlookup[(int(indirectentry[0], 16), int(indirectentry[1]))] = int(indirectentry[2], 16)
indirectfile.close()

allocatedinodes = {}
with open('inode.csv', 'r') as inodefile:
    for line in inodefile:
        inodeentry = line.rstrip('\n').split(',')
        tempinode = Inode(int(inodeentry[0]))
        tempinode.linkcount = int(inodeentry[5])
        inodeblocknum = int(inodeentry[10])
        if int(inodeentry[9]) == 0:
            continue
        for i in range(11, 26):
            tempinode.pointers.append(int(inodeentry[i], 16))
        counter = 0
        for j in range(0, 12):
            forlevel0 = j
            counter = traverse(inodeblocknum, counter, tempinode.pointers[j], 0)
        #print counter, inodeblocknum, tempinode.pointers
        forlevel0 = 12
        counter = traverse(inodeblocknum, counter, tempinode.pointers[12], 0)
        counter = traverse(inodeblocknum, counter, tempinode.pointers[12], 1)
        forlevel0 = 13
        counter = traverse(inodeblocknum, counter, tempinode.pointers[13], 0)
        counter = traverse(inodeblocknum, counter, tempinode.pointers[13], 2)
        forlevel0 = 14
        counter = traverse(inodeblocknum, counter, tempinode.pointers[14], 0)
        counter = traverse(inodeblocknum, counter, tempinode.pointers[14], 3)
        allocatedinodes[tempinode.inodenumber] = tempinode
        #print tempinode.inodenumber, tempinode.linkcount, tempinode.pointers
        #print '\n'
inodefile.close()

dirtable = {}
unallocatedinodes = {}
with open('directory.csv', 'r') as directoryfile:
    while True:
        line = directoryfile.readline()
        if len(line) == 0:
            break;
        direntry = line.rstrip('\n').split(',', 6)
        while direntry[5][-1] != '\"':
            direntry[5] += directoryfile.readline().rstrip('\n')
        if (int(direntry[1]) !=0 and int(direntry[1]) != 1) or int(direntry[0]) == 2:
            dirtable[int(direntry[4])] = int(direntry[0])        
        #if int(direntry[-2]) != int(direntry[0]) || int(direntry[0]) == 2
        if int(direntry[-2]) not in allocatedinodes:
            #ans.write('UNALLOCATED INODE < ' + direntry[-2] + ' > REFERENCED BY DIRECTORY < ' + direntry[0] + ' > ENTRY < ' + direntry[1] + ' >\n')
            if int(direntry[-2]) not in unallocatedinodes:    
                unallocatedinodes[int(direntry[-2])] = [{'dic': int(direntry[0]), 'entry': int(direntry[1])}]
            else:
                unallocatedinodes[int(direntry[-2])].append({'dic': int(direntry[0]), 'entry': int(direntry[1])})
        else:
            allocatedinodes[int(direntry[-2])].referlist.append({'parentinode': int(direntry[0]), 'entrynumber': int(direntry[1])})
            if int(direntry[1]) == 0:
                if int(direntry[-2]) != int(direntry[0]):
                    ans.write('INCORRECT ENTRY IN < ' + direntry[0] + ' > NAME < . > LINK TO < ' + direntry[-2] + ' > SHOULD BE < ' + direntry[0] + ' >\n')
            elif int(direntry[1]) == 1:
                if dirtable[int(direntry[0])] != int(direntry[-2]):
                    ans.write('INCORRECT ENTRY IN < ' + direntry[0] + ' > NAME < .. > LINK TO < ' + direntry[-2] + ' > SHOULD BE < ' + str(dirtable[int(direntry[0])]) + ' >\n')
directoryfile.close()
for inode in unallocatedinodes:
    ans.write('UNALLOCATED INODE < ' + str(inode) + ' > REFERENCED BY')
    referlist = unallocatedinodes[inode]
    for i in range(0, len(referlist)):
        ans.write( ' DIRECTORY < ' + str(referlist[i]['dic']) + ' > ENTRY < ' + str(referlist[i]['entry']) + ' >')
    ans.write('\n')

for inode in allocatedinodes:
    if allocatedinodes[inode].linkcount != len(allocatedinodes[inode].referlist):
        ans.write('LINKCOUNT < '+ str(allocatedinodes[inode].inodenumber) + ' > IS < ' + str(allocatedinodes[inode].linkcount) + ' > SHOULD BE < ' + str(len(allocatedinodes[inode].referlist)) + ' >\n')
for i in range(20, totalinodes):
    if (i not in allocatedinodes) and (i not in inodefreemap):
        ans.write('MISSING INODE < ' + str(i) + ' > SHOULD BE IN FREE LIST < ' + str(inodefreemappos[(i / inodespergroup)]) + ' >\n')

for block in allocatedblocks:
    blockobj = allocatedblocks[block]
    if len(blockobj.referlist) > 1:
        ans.write('MULTIPLY REFERENCED BLOCK < ' + str(block) + ' > BY')
        blockobj = allocatedblocks[block]
        blockreferlist = blockobj.referlist
        for i in range(0, len(blockreferlist)):
            if blockreferlist[i]['indirect_block_num'] == -1:
                ans.write(' INODE < ' + str(blockreferlist[i]['inode']) + ' > ENTRY < ' + str(blockreferlist[i]['entry']) + ' >')
            else:
                ans.write(' INODE < ' + str(blockreferlist[i]['inode']) + ' > INDIRECT BLOCK < ' + str(blockreferlist[i]['indirect_block_num']) + ' >  ENTRY < ' + str(blockreferlist[i]['entry']) + ' >')
        ans.write('\n')
    if block in blockfreemap:
        ans.write('UNALLOCATED BLOCK < ' + str(block) + ' > REFERENCED BY')
        blockobj = allocatedblocks[block]
        blockreferlist = blockobj.referlist
        for i in range(0, len(blockreferlist)):
            if blockreferlist[i]['indirect_block_num'] == -1:
                ans.write(' INODE < ' + str(blockreferlist[i]['inode']) + ' > ENTRY < ' + str(blockreferlist[i]['entry']) + ' >')
            else:
                ans.write(' INODE < ' + str(blockreferlist[i]['inode']) + ' > INDIRECT BLOCK < ' + str(blockreferlist[i]['indirect_block_num']) + ' >  ENTRY < ' + str(blockreferlist[i]['entry']) + ' >')
        ans.write('\n')
# for block in allocatedblocks:
#     blockobj = allocatedblocks[block]
#     print block, blockobj.referlist, '\n'






