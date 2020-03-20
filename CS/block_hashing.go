package main

import (
	"bytes"
	"crypto/sha256"
	"encoding/binary"
	"encoding/hex"
	"log"
	"reflect"
)

type BlockHeader struct {
	Version    uint32
	PrevBlock  string
	MerkleRoot string
	Timestamp  uint32
	Bits       uint32
	Nonce      uint32
}

func uintToBin(num uint32) []byte {
	buf := new(bytes.Buffer)
	err := binary.Write(buf, binary.LittleEndian, num)
	if err != nil {
		log.Fatal("binary.Write failed:", err)
	}
	return buf.Bytes()
}

func strToLittleEndian(str string) string {
	runes := []rune(str)
	for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
		runes[i], runes[j] = runes[j], runes[i]
	}
	for i := 0; i < len(runes)-1; i += 2 {
		runes[i], runes[i+1] = runes[i+1], runes[i]
	}
	return string(runes)
}

func hexToBin(str string) []byte {
	strBytes := []byte(strToLittleEndian(str))
	dst := make([]byte, hex.DecodedLen(len(strBytes)))
	n, err := hex.Decode(dst, strBytes)
	if err != nil {
		log.Fatal(err)
	}
	return dst[:n]
}

func getRawBytes(header *BlockHeader) []byte {
	r := reflect.ValueOf(*header)
	result := []byte{}
	for i := 0; i < r.NumField(); i++ {
		field := r.Field(i)
		var fieldBytes []byte
		if field.Kind() == reflect.Uint32 {
			fieldBytes = uintToBin(field.Interface().(uint32))
		} else {
			fieldBytes = hexToBin(field.Interface().(string))
		}
		log.Println(hex.EncodeToString(fieldBytes))
		result = append(result, fieldBytes...)
	}
	return result
}

func computeHash(header *BlockHeader) string {
	rawBytes := getRawBytes(header)
	innerHash := sha256.Sum256(rawBytes)
	hash := sha256.Sum256(innerHash[:])
	hexStr := hex.EncodeToString(hash[:])
	return strToLittleEndian(hexStr)
}

func main() {
	header := BlockHeader{
		Version:    0x3fff0000,
		PrevBlock:  "0000000000000000000140ac4688aea45aacbe7caf6aaca46f16acd93e1064c3",
		MerkleRoot: "422458fced12693312058f6ee4ada19f6df8b29d8cac425c12f4722e0dc4aafd",
		Timestamp:  0x5E664C76,
		Bits:       0x17110119,
		Nonce:      0x20184C38,
	}
	log.Println(computeHash(&header))
}
