import string
import sys
import itertools

def generate_pattern(length: int):
    combinations = itertools.product(
        string.ascii_uppercase,
        string.ascii_lowercase,
        string.digits
    )
    char_stream = itertools.chain.from_iterable(combinations)
    infinite_chars = itertools.cycle(char_stream)
    return "".join(itertools.islice(infinite_chars, length))

def main():
    if len(sys.argv) != 2:
        print(f"Usage: python3 {sys.argv[0]} <length>")
        return
    length: int = int(sys.argv[1])

    if length < 0:
        print("Error: Length must be positive.")
        return
    print(generate_pattern(length))

if __name__ == "__main__":
    main()
