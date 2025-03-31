#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <iomanip>

using namespace std;

#pragma pack(push, 1)
struct BootSector {
    uint8_t jumpInstruction[3];
    char oemName[8];
    uint16_t bytesPerSector;
    uint8_t sectorsPerCluster;
    uint16_t reservedSectors;
    uint8_t numFATs;
    uint16_t rootEntries;
    uint16_t totalSectors16;
    uint8_t mediaDescriptor;
    uint16_t sectorsPerFAT16;
    uint16_t sectorsPerTrack;
    uint16_t numHeads;
    uint32_t hiddenSectors;
    uint32_t totalSectors32;
    uint32_t sectorsPerFAT;
    uint16_t flags;
    uint16_t version;
    uint32_t rootCluster;
    uint16_t fsInfo;
    uint16_t backupBootSector;
    uint8_t reserved[12];
    uint8_t driveNumber;
    uint8_t reserved1;
    uint8_t bootSignature;
    uint32_t volumeID;
    char volumeLabel[11];
    char fsType[8];
};

struct DirectoryEntry {
    char name[11];
    uint8_t attr;
    uint8_t reserved;
    uint8_t creationTimeTenths;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t lastAccessDate;
    uint16_t firstClusterHigh;
    uint16_t lastModifiedTime;
    uint16_t lastModifiedDate;
    uint16_t firstClusterLow;
    uint32_t fileSize;
};
#pragma pack(pop)

void readBootSector(ifstream& disk, BootSector& bootSector) {
    disk.seekg(0, ios::beg);
    disk.read(reinterpret_cast<char*>(&bootSector), sizeof(BootSector));
}

void printBootSectorInfo(const BootSector& bootSector) {
    cout << "\n======= FAT32 Info =======\n";
    cout << left << setw(25) << "Bytes per Sector:" << bootSector.bytesPerSector << endl;
    cout << left << setw(25) << "Sectors per Cluster:" << (int)bootSector.sectorsPerCluster << endl;
    cout << left << setw(25) << "Reserved Sectors:" << bootSector.reservedSectors << endl;
    cout << left << setw(25) << "Number of FATs:" << (int)bootSector.numFATs << endl;
    cout << left << setw(25) << "Sectors per FAT:" << bootSector.sectorsPerFAT << endl;
    cout << left << setw(25) << "Root Cluster:" << bootSector.rootCluster << endl;
    cout << left << setw(25) << "FAT Start (bytes):" << bootSector.reservedSectors * bootSector.bytesPerSector << endl;
    cout << left << setw(25) << "Data Start (bytes):" << (bootSector.reservedSectors + (bootSector.numFATs * bootSector.sectorsPerFAT)) * bootSector.bytesPerSector << endl;
}

void listFiles(ifstream& disk, const BootSector& bootSector) {
    uint32_t dataStart = (bootSector.reservedSectors + (bootSector.numFATs * bootSector.sectorsPerFAT)) * bootSector.bytesPerSector;
    uint32_t rootDirStart = dataStart + (bootSector.rootCluster - 2) * bootSector.sectorsPerCluster * bootSector.bytesPerSector;

    disk.seekg(rootDirStart, ios::beg);

    cout << "\n======= Files and Directories =======\n";
    cout << left << setw(15) << "Name" << setw(10) << "Type" << setw(15) << "Size (bytes)" << endl;
    cout << string(40, '-') << endl;

    for (int i = 0; i < 16; i++) {
        DirectoryEntry entry;
        disk.read(reinterpret_cast<char*>(&entry), sizeof(DirectoryEntry));

        if (entry.name[0] == 0x00) break;  
        if ((uint8_t)entry.name[0] == 0xE5) continue;  

        string fileName(entry.name, 11);
        fileName.erase(remove(fileName.begin(), fileName.end(), ' '), fileName.end()); 

        cout << left << setw(15) << fileName;
        if (entry.attr & 0x10) {
            cout << setw(10) << "(DIR)";
        }
        else {
            cout << setw(10) << "(FILE)";
            cout << setw(15) << entry.fileSize;
        }
        cout << endl;
    }
}

int main() {
    ifstream disk("\\\\.\\D:", ios::binary);
    if (!disk) {
        cerr << "Failed to open drive." << endl;
        return 1;
    }

    BootSector bootSector;
    readBootSector(disk, bootSector);
    printBootSectorInfo(bootSector);
    listFiles(disk, bootSector);

    disk.close();
    return 0;
}
