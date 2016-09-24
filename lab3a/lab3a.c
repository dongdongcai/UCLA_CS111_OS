#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#define SUPERBLOCK 1024
#define DESCSIZE 32
#define PTRNUMBER 15
#define BYTESIZE 8
#define FIRSTINODENUMBER 1

struct super_block {
	uint16_t magic;
	uint32_t totalinodes;
	uint32_t totalblocks;
	uint32_t blocksize;
	uint32_t fragmentsize;
	uint32_t blockspergroup;
	uint32_t inodespergroup;
	uint32_t fragspergroup;
	uint32_t firstdatablock;
	uint16_t inodesize;
};

struct group_desc {
	uint32_t containedblocks;
	uint16_t freeblockscount;
	uint16_t freeinodescount;
	uint16_t numofdirs;
	uint32_t inodebitmap;
	uint32_t blockbitmap;
	uint32_t inodetable;
};

struct inode {
	uint32_t inodenumber;
	char filetype;
	uint16_t mode;
	uint32_t uid;
	uint32_t gid;
	uint16_t linkcount;
	uint32_t crtime;
	uint32_t mtime;
	uint32_t atime;
	uint32_t filesize;
	uint32_t blocksnumber;
	uint32_t ptrtoblocks[PTRNUMBER];
	//uint32_t indiptr;
	//uint32_t doubleindiptr;
	//uint32_t triindirptr;
};

struct group_desc* groups;
struct inode* inodesarray;
struct super_block* super;

int dirinfo,
	indirinfo,
	fd;

void findEntry(uint32_t thisinode, uint32_t thisblock, int flag, int* count) {
	if(thisblock == 0) return;
	if(flag == 0) {
		uint32_t blockoff = 0;
		uint32_t thisoffset = thisblock * super->blocksize;
		while(blockoff < super->blocksize) {	
			uint32_t entryinode;
			uint16_t entrylength;
			uint8_t  namelength;
			char*	 filename;
			pread(fd, &entryinode, 4, thisoffset + blockoff);
			pread(fd, &entrylength, 2, thisoffset + blockoff + 4);
			pread(fd, &namelength, 1, thisoffset + blockoff + 6);
			filename = (char*)malloc(namelength + 1);
			pread(fd, filename, namelength, thisoffset + blockoff + 8);
			filename[namelength] = '\0';
			if(entryinode != 0) {
				dprintf(dirinfo, "%d,%d,%d,%d,%d,\"%s\"\n", thisinode, *count, entrylength, namelength, entryinode, filename);
			}
			*count += 1;
			blockoff += entrylength;
			//printf("%d\n", blockoff);
			//if(*count == 35) exit(0);
		}
	} else {
		int i;
		uint32_t thisoffset = thisblock * super->blocksize;		
		for(i = 0; i < super->blocksize; i = i + 4) {
			uint32_t indirblock;
			pread(fd, &indirblock, 4, thisoffset + i);
			findEntry(thisinode, indirblock, flag - 1, count);
		}
	}
}

void findBlock(uint32_t thisblock, int flag) {
	if(thisblock == 0) return;
	int i;
	uint32_t thisoffset = thisblock * super->blocksize;
	int count = 0;
	for(i = 0; i < super->blocksize; i = i + 4) {
		uint32_t indirblock;
		pread(fd, &indirblock, 4, thisoffset + i);
		if(indirblock != 0) {
			dprintf(indirinfo, "%x,%d,%x\n", thisblock, count++, indirblock);
		}
	}
	if(flag > 0) {
		for(i = 0; i < super->blocksize; i = i + 4) {
			uint32_t indirblock;
			pread(fd, &indirblock, 4, thisoffset + i);
			findBlock(indirblock, flag - 1);
		}
	}
}

int main(int argc, char* argv[]) {
	int i;
	fd = open("disk-image", O_RDONLY);
	if(fd == -1) {
		fprintf(stderr, "open() disk-image failed!\n");
		exit(1);
	}
	super = (struct super_block*)malloc(sizeof(struct super_block));
	if(super == NULL) {
		fprintf(stderr, "malloc() for superblock failed!\n");
		exit(1);
	}
	pread(fd, &super->magic, 2, SUPERBLOCK + 56);
	pread(fd, &super->totalinodes, 4, SUPERBLOCK + 0);
	pread(fd, &super->totalblocks, 4, SUPERBLOCK + 4);
	pread(fd, &super->blocksize, 4, SUPERBLOCK + 24);
	super->blocksize = 1024 << super->blocksize;
	pread(fd, &super->fragmentsize, 4, SUPERBLOCK + 28);
	super->fragmentsize = 1024 << super->fragmentsize;
	pread(fd, &super->firstdatablock, 4, SUPERBLOCK + 20);		
	pread(fd, &super->blockspergroup, 4, SUPERBLOCK + 32);		
	pread(fd, &super->fragspergroup, 4, SUPERBLOCK + 36);		
	pread(fd, &super->inodespergroup, 4, SUPERBLOCK + 40);
	pread(fd, &super->inodesize, 2, SUPERBLOCK + 88);
	int superblockfd = creat("super.csv", 0644);
	if(superblockfd == -1) {
		fprintf(stderr, "open() super.csv failed!\n");
		exit(1);
	}
	dprintf(superblockfd, "%x,%d,%d,%d,%d,%d,%d,%d,%d\n", super->magic, super->totalinodes, super->totalblocks, super->blocksize, super->fragmentsize, super->blockspergroup, super->inodespergroup, super->fragspergroup, super->firstdatablock);
	close(superblockfd);
	
	//struct group_desc* groups;
	//struct inode* inodes;
	const uint32_t DESCRIPTOR = (super->firstdatablock + 1) * super->blocksize;
	int numofgroups = 1 + (super->totalblocks - super->firstdatablock) / super->blockspergroup;
	groups = (struct group_desc*)malloc(numofgroups * sizeof(struct group_desc));
	if(groups == NULL) {
		fprintf(stderr, "malloc() for groups failed!\n");
		exit(1);		
	}
	inodesarray = (struct inode*)malloc(super->totalinodes * sizeof(struct inode));
	if(inodesarray == NULL) {
		fprintf(stderr, "malloc() for inodes failed!\n");
		exit(1);		
	}
	int groupinfo = creat("group.csv", 0644);
	if(groupinfo == -1) {
		fprintf(stderr, "open() group.csv failed!\n");
		exit(1);
	}
	int freeinfo = creat("bitmap.csv", 0644);
	if(freeinfo == -1) {
		fprintf(stderr, "open() bitmap.csv failed!\n");
		exit(1);
	}
	int inodeinfo = creat("inode.csv", 0644);
	if(freeinfo == -1) {
		fprintf(stderr, "open() inode.csv failed!\n");
		exit(1);
	}
	struct inode* inodes = inodesarray;
	for(i = 0; i < numofgroups; i++) {
		uint32_t currentdesc = DESCRIPTOR + i * DESCSIZE;
		pread(fd, &(groups[i].blockbitmap), 4, currentdesc + 0);
		pread(fd, &(groups[i].inodebitmap), 4, currentdesc + 4);
		pread(fd, &(groups[i].inodetable), 4, currentdesc + 8);
		pread(fd, &(groups[i].freeblockscount), 2, currentdesc + 12);
		pread(fd, &(groups[i].freeinodescount), 2, currentdesc + 14);
		pread(fd, &(groups[i].numofdirs), 2, currentdesc + 16);
		groups[i].containedblocks = super->blockspergroup;
		if(i == numofgroups - 1) groups[i].containedblocks = super->totalblocks % super->blockspergroup;
		dprintf(groupinfo, "%d,%d,%d,%d,%x,%x,%x\n", groups[i].containedblocks, groups[i].freeblockscount, groups[i].freeinodescount, groups[i].numofdirs, groups[i].inodebitmap, groups[i].blockbitmap, groups[i].inodetable);		
		uint32_t currentbitmap = groups[i].blockbitmap * super->blocksize;
		int j, k;
		for(j = 0; j < groups[i].containedblocks / BYTESIZE; j++) {
			char temp;
			pread(fd, &temp, 1, currentbitmap + j);
			for(k = 0; k < BYTESIZE; k++) {
				if((temp & 1) == 0) {
					dprintf(freeinfo, "%x,%d\n", groups[i].blockbitmap, 8 * j + k + super->firstdatablock + super->blockspergroup * i);
				} 
				temp = temp >> 1;
			}
		}
		currentbitmap = groups[i].inodebitmap * super->blocksize;
		for(j = 0; j < super->inodespergroup / BYTESIZE; j++) {
			char temp;
			pread(fd, &temp, 1, currentbitmap + j);
			for(k = 0; k < BYTESIZE; k++) {
				if((temp & 1) == 0) {
					dprintf(freeinfo, "%x,%d\n", groups[i].inodebitmap, 8 * j + k + FIRSTINODENUMBER + super->inodespergroup * i);
				} else {
					int m;
					uint32_t tempid;
					uint32_t inodeoff = groups[i].inodetable * super->blocksize + (8 * j + k) * (super->inodesize);
					inodes->inodenumber = 8 * j + k + FIRSTINODENUMBER + super->inodespergroup * i;
					pread(fd, &inodes->mode, 2, inodeoff);
					//printf("%x\n", inodes->mode);
					if((inodes->mode & 0xA000) == 0xA000) {
						inodes->filetype = 's';
					} else if((inodes->mode & 0x8000) == 0x8000) {
						inodes->filetype = 'f';
					} else if((inodes->mode & 0x4000) == 0x4000) {
						inodes->filetype = 'd';
					} else {
						inodes->filetype = '?';
					}
					pread(fd, &inodes->uid, 2, inodeoff + 2);
					pread(fd, &tempid, 2, inodeoff + 116 + 4);
					inodes->uid = tempid << 16 | inodes->uid;
					pread(fd, &inodes->filesize, 4, inodeoff + 4);
					pread(fd, &inodes->atime, 4, inodeoff + 8);
					pread(fd, &inodes->crtime, 4, inodeoff + 12);
					pread(fd, &inodes->mtime, 4, inodeoff + 16);
					pread(fd, &inodes->gid, 2, inodeoff + 24);
					pread(fd, &tempid, 2, inodeoff + 116 + 6);
					inodes->gid = tempid << 16 | inodes->gid;
					pread(fd, &inodes->linkcount, 2, inodeoff + 26);
					pread(fd, &inodes->blocksnumber, 4, inodeoff + 28);
					inodes->blocksnumber = inodes->blocksnumber / (super->blocksize / 512);
					for(m = 0; m < PTRNUMBER; m++) {
						pread(fd, inodes->ptrtoblocks + m, 4, inodeoff + 40 + m * 4);

					}
					dprintf(inodeinfo, "%d,%c,%o,%d,%d,%d,%x,%x,%x,%d,%d",
							inodes->inodenumber, inodes->filetype, inodes->mode, inodes->uid, inodes->gid, inodes->linkcount, inodes->crtime, inodes->mtime, inodes->atime, inodes->filesize, inodes->blocksnumber);
					for(m = 0; m < PTRNUMBER; m++) {
						dprintf(inodeinfo, ",%x", inodes->ptrtoblocks[m]);
					}
					dprintf(inodeinfo, "\n");
					inodes += 1;
				}
				temp = temp >> 1;
			}
		}

	}
	close(groupinfo);
	close(inodeinfo);
	close(freeinfo);

	dirinfo = creat("directory.csv", 0644);
	if(dirinfo == -1) {
		fprintf(stderr, "open() directory.csv failed!\n");
		exit(1);
	}
	indirinfo = creat("indirect.csv", 0644);
	if(indirinfo == -1) {
		fprintf(stderr, "open() indirect.csv failed!\n");
		exit(1);
	}
	inodes = inodesarray;		
	while(inodes->inodenumber != 0) {
		if(inodes->filetype == 'd') {
			//printf("nmsl\n");
			int bcount = 0;
			for(i = 0; i < 12; i++) {
				findEntry(inodes->inodenumber, inodes->ptrtoblocks[i], 0, &bcount);
			}
			for(i = 12; i < PTRNUMBER; i++) {
				findEntry(inodes->inodenumber, inodes->ptrtoblocks[i], i - 11, &bcount);
			}
		}
		int j;
		for(j = 12; j < PTRNUMBER; j++) {
			findBlock(inodes->ptrtoblocks[j], j - 12);
		}
		inodes += 1;
	}
	close(dirinfo);
	close(indirinfo);
	return 0;				
}