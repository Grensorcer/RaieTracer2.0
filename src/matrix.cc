#include "matrix.hh"

namespace structures
{
    FixedMatrix<1ul, 3ul> operator^(const FixedMatrix<1ul, 3ul> &lhs,
                                    const FixedMatrix<1ul, 3ul> &rhs)
    {
        return FixedMatrix<1ul, 3ul>({ { lhs[1] * rhs[2] - lhs[2] * rhs[1],
                                         lhs[2] * rhs[0] - lhs[0] * rhs[2],
                                         lhs[0] * rhs[1] - lhs[1] * rhs[0] } });
    }
} // namespace structures
