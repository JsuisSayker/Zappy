def lookingSizeOfMap(array: list) -> int:
    """
    The function `lookingSizeOfMap` returns the index of the first occurrence
    of an empty list in the input list `array`, or the length of the list if
    no empty list is found.

    @param array The `array` parameter in the `lookingSizeOfMap` function is
    expected to be a list of elements. The function searches for the first
    occurrence of an empty list `[]` within the input list and returns its
    index. If no empty list is found in the input list, the function returns
    the length

    @return The code snippet is returning the index of the first occurrence of
    an empty list `[]` in the input `array`. If no empty list is found in the
    array, it returns the length of the array.
    """
    return next((i for i, elem in enumerate(array) if elem == []),
                len(array))
