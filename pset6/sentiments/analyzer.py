import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # declare sets for positive and negative words
        self.positives = set()
        self.negatives = set()

        # load positive words
        with open(positives) as lines:
            for line in lines:
                if not line.startswith(";") and not line.startswith(" "):
                    self.positives.add(line.strip())

        # load negative words
        with open(negatives) as lines:
            for line in lines:
                if not line.startswith(";") and not line.startswith(" "):
                    self.negatives.add(line.strip())

        self.tokenizer = nltk.tokenize.TweetTokenizer()
        self.score = 0


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        tokens = self.tokenizer.tokenize(text)

        for token in tokens:
            token = token.lower()

            if token in self.positives:
                self.score += 1
            elif token in self.negatives:
                self.score -= 1

        return self.score
