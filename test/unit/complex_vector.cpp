//          Copyright Heikki Berg 2017 - 2018
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include <gtest/gtest.h>

#include <cdsp/dsp_types.h>
#include <cdsp/complex.h>
#include <cdsp/complex_vector.h>
#include <cdsp/virtual_float.h>


TEST(complex_vector, vector)
{
    cdsp::complex_vector<float> vec(10, cdsp::complex<float>(1.0f, 2.0f));

    EXPECT_EQ(static_cast<int>(vec.size()), 10);
    std::vector<float>& real = vec.real_data();
    std::vector<float>& imag = vec.imag_data();
    for (float r : real)
    {
        EXPECT_EQ(r, 1.0f);
    }
    for (float i : imag)
    {
        EXPECT_EQ(i, 2.0f);
    }

    vec.set_at(3, cdsp::complex<float>(3.0f));
    for (std::size_t index = 0; index < 10; index++)
    {
        if (index == 3)
        {
            EXPECT_EQ(vec.at(index).real(), 3.0f);
            EXPECT_EQ(vec.at(index).imag(), 0.0f);
        }
        else
        {
            EXPECT_EQ(vec.at(index).real(), 1.0f);
            EXPECT_EQ(vec.at(index).imag(), 2.0f);
        }
    }
    for (float& i : imag)
    {
        i = 5.0f;
    }
    for (std::size_t index = 0; index < 10; index++)
    {
        if (index == 3)
        {
            EXPECT_EQ(vec.at(index).real(), 3.0f);
            EXPECT_EQ(vec.at(index).imag(), 5.0f);
        }
        else
        {
            EXPECT_EQ(vec.at(index).real(), 1.0f);
            EXPECT_EQ(vec.at(index).imag(), 5.0f);
        }
    }
}

TEST(complex_vector, vector_add)
{
    cdsp::complex_vector<float> vec1(10, cdsp::complex<float>(1.0f, 2.0f));
    cdsp::complex_vector<float> vec2(10, cdsp::complex<float>(2.0f, 7.0f));

    cdsp::complex_vector<float> vec = vec1 + vec2;

    std::vector<float> const& real = vec.real();
    std::vector<float> const& imag = vec.imag();
    for (float const& r : real)
    {
        EXPECT_EQ(r, 3.0f);
    }
    for (float const& i : imag)
    {
        EXPECT_EQ(i, 9.0f);
    }
}

TEST(complex_vector, vector_substract)
{
    cdsp::complex_vector<float> vec1(10, cdsp::complex<float>(1.0f, 2.0f));
    cdsp::complex_vector<float> vec2(10, cdsp::complex<float>(2.0f, 7.0f));

    cdsp::complex_vector<float> vec = vec1 - vec2;

    std::vector<float> const& real = vec.real();
    std::vector<float> const& imag = vec.imag();
    for (float r : real)
    {
        EXPECT_EQ(r, -1.0f);
    }
    for (float i : imag)
    {
        EXPECT_EQ(i, -5.0f);
    }
    vec += vec2;
    for (float r : real)
    {
        EXPECT_EQ(r, 1.0f);
    }
    for (float i : imag)
    {
        EXPECT_EQ(i, 2.0f);
    }
}

TEST(complex_vector, vector_conjugate)
{
    cdsp::complex_vector<float> vec(10, cdsp::complex<float>(1.0f, 2.0f));

    std::vector<float> const& real = vec.real();
    std::vector<float> const& imag = vec.imag();
    for (float r : real)
    {
        EXPECT_EQ(r, 1.0f);
    }
    for (float i : imag)
    {
        EXPECT_EQ(i, 2.0f);
    }
    vec.conj();
    for (float r : real)
    {
        EXPECT_EQ(r, 1.0f);
    }
    for (float i : imag)
    {
        EXPECT_EQ(i, -2.0f);
    }

    cdsp::complex_vector<float> vec2 = conj(vec);
    std::vector<float> const& re = vec2.real();
    std::vector<float> const& im = vec2.imag();
    for (float r : re)
    {
        EXPECT_EQ(r, 1.0f);
    }
    for (float i : im)
    {
        EXPECT_EQ(i, 2.0f);
    }
}

TEST(complex_vector, vector_abs)
{
    cdsp::complex_vector<float> vec(10, cdsp::complex<float>(1.0f, 2.0f));

    std::vector<float> absoluteValues = abs(vec);

    for (float val : absoluteValues)
    {
        EXPECT_EQ(val, 2.23606797749979f);
    }
}

TEST(complex_vector, vector_hadamard)
{
    cdsp::complex_vector<float> vec1(10, cdsp::complex<float>(1.5f, 2.0f));
    cdsp::complex_vector<float> vec2(10, cdsp::complex<float>(2.0f, 7.0f));

    cdsp::complex_vector<float> res = hadamard(vec1, vec2);

    std::vector<float> const& re = res.real();
    std::vector<float> const& im = res.imag();
    for (float r : re)
    {
        EXPECT_EQ(r, -11.0f);
    }
    for (float i : im)
    {
        EXPECT_EQ(i, 14.5f);
    }
}

TEST(complex_vector, vector_dot)
{
    cdsp::complex_vector<float> vec1(10, cdsp::complex<float>(1.5f, 2.0f));
    cdsp::complex_vector<float> vec2(10, cdsp::complex<float>(2.0f, 7.0f));

    {
        cdsp::complex<float> res = cdsp::dot<float, float>(vec1, vec2);
        EXPECT_EQ(res.real(), 170.0f);
        EXPECT_EQ(res.imag(), -65.0f);
    }
    {
        cdsp::complex<float> res = cdsp::dot<float, float>(vec1, vec2, true);
        EXPECT_EQ(res.real(), -110.0f);
        EXPECT_EQ(res.imag(), 145.0f);
    }
}

//TEST(complex_vector, vector_virtual_float_dot)
//{
//    using cdsp::complex;
//    using cdsp::complex_vector;
//    using cdsp::dot;
//    using cdsp::virtual_float;
//
//    complex_vector<float> vec1(10, complex<float>(0.125f, 0.250f));
//    complex_vector<float> vec2(10, complex<float>(0.250f, 0.625f));
//    complex_vector<q4_20> vece1(10, complex<q4_20>(0.125f, 0.250f));
//    complex_vector<q4_20> vece2(10, complex<q4_20>(0.250f, 0.625f));
//
//    {
//        complex<virtual_float<float> > vf = dot<float, virtual_float<float> >(vec1, vec2);
//        EXPECT_EQ(static_cast<float>(vf.real()), 1.875f);
//        EXPECT_EQ(static_cast<float>(vf.imag()), -0.15625f);
//    }
//
//    {
//        complex<virtual_float<float> > vf = dot<float, virtual_float<float> >(vec1, vec2, true);
//        EXPECT_EQ(static_cast<float>(vf.real()), -1.25f);
//        EXPECT_EQ(static_cast<float>(vf.imag()), 1.40625f);
//    }
//    {
//        complex<virtual_float<q4_20> > vf = dot<q4_20, virtual_float<q4_20> >(vece1, vece2);
//        EXPECT_EQ(static_cast<float>(vf.real()), 1.875f);
//        EXPECT_EQ(static_cast<float>(vf.imag()), -0.15625f);
//        cdsp::complex<float> res = vf;
//        EXPECT_EQ(res.real(), 1.875f);
//        EXPECT_EQ(res.imag(), -0.15625f);
//    }
//}
