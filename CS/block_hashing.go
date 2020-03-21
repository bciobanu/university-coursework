package main

import (
	"bytes"
	"crypto/sha256"
	"encoding/binary"
	"encoding/hex"
	"log"
	"math/rand"
	"reflect"
	"strings"
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

func getRawBytes(header BlockHeader) []byte {
	r := reflect.ValueOf(header)
	result := []byte{}
	for i := 0; i < r.NumField(); i++ {
		field := r.Field(i)
		var fieldBytes []byte
		if field.Kind() == reflect.Uint32 {
			fieldBytes = uintToBin(field.Interface().(uint32))
		} else {
			fieldBytes = hexToBin(field.Interface().(string))
		}
		result = append(result, fieldBytes...)
	}
	return result
}

func computeHash(header BlockHeader) string {
	rawBytes := getRawBytes(header)
	innerHash := sha256.Sum256(rawBytes)
	hash := sha256.Sum256(innerHash[:])
	hexStr := hex.EncodeToString(hash[:])
	return strToLittleEndian(hexStr)
}

func testExampleHeader() {
	header := BlockHeader{
		Version:    0x3fff0000,
		PrevBlock:  "0000000000000000000140ac4688aea45aacbe7caf6aaca46f16acd93e1064c3",
		MerkleRoot: "422458fced12693312058f6ee4ada19f6df8b29d8cac425c12f4722e0dc4aafd",
		Timestamp:  0x5E664C76,
		Bits:       0x17110119,
		Nonce:      0x20184C38,
	}
	hash := computeHash(header)
	if hash != "0000000000000000000d493c3c1b91c8059c6b0838e7e68fbcf8f8382606b82c" {
		log.Fatal("Example block hash failed: ", hash)
	}
}

func getDifficulty(nBits uint32) string {
	// https://bitcoin.org/en/developer-reference#target-nbits
	// https://bitcoin.stackexchange.com/questions/71578/understanding-bits-and-difficulty-in-a-block-header
	const NumMantissaBits = 32 - 8
	const MantissaMask = (1 << NumMantissaBits) - 1

	exponent := 2 * ((nBits >> NumMantissaBits) - 3)
	mantissa := uintToBin(nBits & MantissaMask)
	mantissaStr := hex.EncodeToString(mantissa)
	mantissaStr = strToLittleEndian(mantissaStr)
	mantissaStr = mantissaStr + strings.Repeat("0", int(exponent))
	mantissaStr = strings.Repeat("0", 64-len(mantissaStr)) + mantissaStr
	return mantissaStr
}

func testDifficultyValue() {
	difficulty := getDifficulty(0x181bc330)
	if difficulty != "00000000000000001bc330000000000000000000000000000000000000000000" {
		log.Fatal("Difficulty calculation failed: ", difficulty)
	}
}

func solve(startNonce uint32) uint32 {
	const NumIterations uint32 = 100_000_000
	const NumConsumers uint32 = 12
	const NumConsumerIterations = (NumIterations + NumConsumers - 1) / NumConsumers

	header := BlockHeader{
		Version:    0x20400000,
		PrevBlock:  "00000000000000000006a4a234288a44e715275f1775b77b2fddb6c02eb6b72f",
		MerkleRoot: "2dc60c563da5368e0668b81bc4d8dd369639a1134f68e425a9a74e428801e5b8",
		Timestamp:  0x5DB8AB5E,
		Bits:       0x17148EDF,
	}

	result := make(chan uint32)
	difficulty := getDifficulty(header.Bits)
	for consumer := uint32(0); consumer < NumConsumers; consumer++ {
		from := startNonce + consumer*NumConsumerIterations
		to := from + NumConsumerIterations
		if to > startNonce+NumIterations {
			to = startNonce + NumIterations
		}

		go func(header BlockHeader, from, to uint32) {
			for nonce := from; nonce < to; nonce++ {
				header.Nonce = nonce
				hash := computeHash(header)
				if hash < difficulty {
					result <- header.Nonce
					break
				}
			}
			result <- 0
		}(header, from, to)
	}

	for i := uint32(0); i < NumConsumers; i++ {
		consumerResult := <-result
		if consumerResult != 0 {
			return consumerResult
		}
	}
	return 0
}

func main() {
	testExampleHeader()
	testDifficultyValue()
	log.Println("Finished testing..")

	firstCase := solve(3_000_000_000) // 3060331852
	log.Println("First case", firstCase)

	for warmup := 0; warmup < 100; warmup++ {
		rand.Int()
	}
	lookAhead := uint32(rand.Intn(1 << 30))
	log.Println("lookAhead is", lookAhead)

	secondCase := solve(firstCase + lookAhead)
	log.Println("Second case", secondCase)
}
