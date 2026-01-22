import sys
import matplotlib.pyplot as plt
from collections import Counter

def main():
    data_file = sys.argv[1]
    output_file = sys.argv[2]
    title = sys.argv[3] if len(sys.argv) > 3 else "Text Statistics"

    with open(data_file, 'r') as f:
        text = f.read()

    # Count character frequencies
    char_counts = Counter(text)
    chars = list(char_counts.keys())
    counts = list(char_counts.values())

    plt.figure(figsize=(10, 6))
    plt.bar(chars, counts)
    plt.xlabel('Characters')
    plt.ylabel('Frequency')
    plt.title(title)
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig(output_file)
    plt.close()

if __name__ == '__main__':
    main()
