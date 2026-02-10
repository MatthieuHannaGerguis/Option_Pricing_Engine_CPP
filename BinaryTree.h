#ifndef PROJECT_BINARYTREE_H
#define PROJECT_BINARYTREE_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>    // for fabs
#include <stdexcept>

template<typename T> // template lets you write code that works with any data type

class BinaryTree {
private:
    int _depth = 0;                     // member depth that represents N
    std::vector<std::vector<T>> _tree;  // holds values per level

    static int countDigits(double num) {
        int intPart = static_cast<int>(std::abs(num));
        int digits = 0;

        while (intPart != 0) {
            intPart /= 10;
            digits++;
        }

        return (digits == 0 ? 1 : digits);
    }

public:

    BinaryTree() = default;     // default constructor


    explicit BinaryTree(int depth) {
        setDepth(depth);
    }

  
    ~BinaryTree() = default;   // destructor (clean & explicit)

    // we set the depth of the tree and resizes all levels
    void setDepth(int depth) {
        if (depth < 0) throw std::invalid_argument("Depth must be nonnegative");

        _depth = depth;
        _tree.clear();
        _tree.resize(_depth + 1); // resizes the tree

        for (int n = 0; n <= _depth; ++n)
            _tree[n].resize(n + 1); // allocates internal vectors
    }

    int getDepth() const { return _depth; }

    void setNode(int n, int i, const T& value) {
        if (n < 0 || n > _depth || i < 0 || i > n)
            throw std::out_of_range("Invalid node indices");

        _tree[n][i] = value;
    }

    const T& getNode(int n, int i) const {
        if (n < 0 || n > _depth || i < 0 || i > n)
            throw std::out_of_range("Invalid node indices");

        return _tree[n][i];
    }

    // this displays the tree in a readable triangular form
    void display() const {
        std::cout << "\n--- Displaying Binary Tree (depth = " << _depth << ") ---\n\n";
        if (_depth < 0) return;

        int width = 8;  // spacing between numbers

        for (int n = 0; n <= _depth; ++n) {
            int indent = (_depth - n) * width / 2;
            std::cout << std::setw(indent) << "";

            for (int i = 0; i <= n; ++i) {
                double value = _tree[n][i];
                std::cout << std::setw(width);

                // we print "0" for very small or negative-zero values
                if (std::fabs(value) < 1e-9)
                    std::cout << " 0 ";
                else
                    std::cout << std::fixed << std::setprecision(2) << value;
            }
            std::cout << "\n";

            // we print the link lines except after the last row
            if (n < _depth) {
                int linkIndent = (_depth - n) * (width / 2) + width / 5;
                std::cout << std::setw(linkIndent) << "";

                for (int i = 0; i <= n; ++i) {
                    std::cout << std::setw(width / 2) << "/"
                        << std::setw(width / 2) << "\\";
                }
                std::cout << "\n";
            }
        }
        std::cout << std::endl;
    }
};

#endif
