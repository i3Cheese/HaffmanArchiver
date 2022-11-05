#include <iterator>

template <class _RandomAccessIterator, class _Compare>
constexpr void SiftUp(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp,
                      typename std::iterator_traits<_RandomAccessIterator>::difference_type len) {
    using ValueType = typename std::iterator_traits<_RandomAccessIterator>::value_type;
    if (len > 1) {
        len = (len - 2) / 2;
        _RandomAccessIterator ptr = first + len;
        if (comp(*ptr, *--last)) {
            ValueType t(std::move(*last));
            do {
                *last = std::move(*ptr);
                last = ptr;
                if (len == 0) {
                    break;
                }
                len = (len - 1) / 2;
                ptr = first + len;
            } while (comp(*ptr, t));
            *last = std::move(t);
        }
    }
}

template <class _Compare, class _RandomAccessIterator>
constexpr void SiftDown(_RandomAccessIterator first, _RandomAccessIterator /*__last*/, _Compare comp,
                        typename std::iterator_traits<_RandomAccessIterator>::difference_type len,
                        _RandomAccessIterator start) {
    using DifferenceType = typename std::iterator_traits<_RandomAccessIterator>::difference_type;
    using ValueType = typename std::iterator_traits<_RandomAccessIterator>::value_type;
    // left-child of start is at 2 * start + 1
    // right-child of start is at 2 * start + 2
    DifferenceType child = start - first;

    if (len < 2 || (len - 2) / 2 < child) {
        return;
    }

    child = 2 * child + 1;
    _RandomAccessIterator child_i = first + child;

    if ((child + 1) < len && comp(*child_i, *(child_i + 1))) {
        // right-child exists and is greater than left-child
        ++child_i;
        ++child;
    }

    // check if we are in heap-order
    if (comp(*child_i, *start)) {
        // we are, __start is larger than it's largest child
        return;
    }

    ValueType top(std::move(*start));
    do {
        // we are not in heap-order, swap the parent with its largest child
        *start = std::move(*child_i);
        start = child_i;

        if ((len - 2) / 2 < child) {
            break;
        }

        // recompute the child based off of the updated parent
        child = 2 * child + 1;
        child_i = first + child;

        if ((child + 1) < len && comp(*child_i, *(child_i + 1))) {
            // right-child exists and is greater than left-child
            ++child_i;
            ++child;
        }

        // check if we are in heap-order
    } while (!comp(*child_i, top));
    *start = std::move(top);
}

template <class _RandomAccessIterator, class _Compare>
inline constexpr void PushHeap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp) {
    SiftUp(first, last, comp, last - first);
}

template <class _RandomAccessIterator>
inline constexpr void PushHeap(_RandomAccessIterator first, _RandomAccessIterator last) {
    PushHeap(first, last, std::less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}

template <class _Compare, class _RandomAccessIterator>
inline constexpr void PopHeap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp) {
    auto len = last - first;
    if (len > 1) {
        std::swap(*first, *--last);
        SiftDown<_Compare>(first, last, comp, len - 1, first);
    }
}

template <class _RandomAccessIterator>
inline constexpr void PopHeap(_RandomAccessIterator first, _RandomAccessIterator last) {
    PopHeap(first, last, std::less<typename std::iterator_traits<_RandomAccessIterator>::value_type>());
}
