from itertools import cycle


def xorStrings(firstString: str, secondString: str):
    """
    The function `xorStrings` takes two strings as input and performs a
    bitwise XOR operation on each character pair from the two strings.

    @param firstString The `firstString` parameter in the `xorStrings`
    function is a string that will be used to XOR (exclusive OR) with the
    characters of the `secondString` parameter.
    @param secondString The `secondString` parameter is the string that will be
    XOR-ed with the `firstString` parameter character by character.

    @return The function `xorStrings` is returning a string that is the result
    of performing a bitwise XOR operation between each character of
    `secondString` and the corresponding character from `firstString`.
    """
    return ''.join(chr(ord(c) ^ ord(k)) for c, k in zip(secondString, cycle(
        firstString)))
