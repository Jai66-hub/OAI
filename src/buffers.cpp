#include <buffers.h>

#include <sstream>
#include <iostream>
#include <iomanip>

const std::string convert_to_string(const std::vector<uint8_t> &buffer)
{
    return std::string(buffer.begin(), buffer.end());
};

std::ostream &operator<<(std::ostream &output, std::vector<uint8_t> &data)
{
    for (const auto &el : data)
    {
        output << el;
    }
    return output;
}


// Wireshark understands a hexdump of the form generated by od -Ax -tx1 -v
/*
000000 00 e0 1e a7 05 6f 00 10 ........
000008 5a a0 b9 12 08 00 46 00 ........
000010 03 68 00 00 00 00 0a 2e ........
000018 ee 33 0f 19 08 7f 0f 19 ........
000020 03 80 94 04 00 00 10 01 ........
000028 16 a2 0a 00 03 50 00 0c ........
000030 01 01 0f 19 03 80 11 01 ........
*/

std::string dump_wireshark_with_ngap_encapsulation(const std::vector<uint8_t> &data)
{
    std::vector<uint8_t> packet =
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, // ethernet
         0x45, 0x02, 0x00, 0x50, 0x00, 0x01, 0x40, 0x00, 0x40, 0x84, 0x3c, 0x25, 0x7f, 0x00, 0x00, 0x01,
         0x7f, 0x00, 0x00, 0x01, 0xdf, 0xb0, 0x96, 0x0c, 0xea, 0x4c, 0xf5, 0x4b, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x03, 0x00, 0x30, 0x20, 0xbc, 0xec, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x0f, 0x40, 0x1c, 0x00, 0x00, 0x01, 0x00, 0x26, 0x00, 0x15, 0x14};

    // FIXME overflow if size > uint8_t
    const uint8_t nas_size = data.size();
    //packet.push_back(size + 1);
    //packet.push_back(size);
    // Add nas packet
    std::move(data.begin(), data.end(), std::back_inserter(packet));

    // recalculate sizes for layers
    // ipv4
    packet[17] = packet.size() - 14; // 14 = ethernet header size
    // sctp
    packet[49] = packet.size() - 14 - 20 - 12; // 20 = ipv4, 12 = sctp
    // pdu-nas
    packet[72] = nas_size + 1;
    packet[73] = nas_size;
    packet[65] = nas_size + 8;

    return dump_wireshark(packet);
}

std::string dump_wireshark(const std::vector<uint8_t> &data)
{
    std::string result;
    std::stringstream ss;
    const unsigned int width = 8;
    const unsigned int address_size = 5;

    unsigned int offset = 0;

    while (offset < data.size())
    {
        std::vector<uint8_t>::const_iterator end;
        if ((offset + width) >= data.size())
        {
            end = data.cend();
        }
        else
        {
            end = data.cbegin() + offset + width;
        }

        std::vector<uint8_t> row(data.cbegin() + offset, end);
        // address
        ss << std::setfill('0') << std::setw(address_size) << std::hex << offset;
        ss << " ";
        // hex
        for (const auto &byte : row)
        {
            ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(byte);
            ss << " ";
        }
        for (unsigned int i = row.size(); i < width; i++)
        {
            ss << "   ";
        }
        // ascii
        for(const auto &byte : row)
        {
            if (byte >= 32 && byte < 127)
            {
                ss << std::setfill('.') << std::setw(1) << byte;
            }
            else
            {
                ss << ".";
            }
        }
        ss << std::endl;
        offset += width;
    }
    return ss.str();
}