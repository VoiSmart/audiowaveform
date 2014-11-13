//------------------------------------------------------------------------------
//
// Copyright 2013, 2014 BBC Research and Development
//
// Author: Chris Needham
//
// This file is part of Audio Waveform Image Generator.
//
// Audio Waveform Image Generator is free software: you can redistribute it
// and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Audio Waveform Image Generator is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// Audio Waveform Image Generator.  If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------------------------------------------------------

#include "Options.h"
#include "Array.h"
#include "util/Streams.h"

#include "gmock/gmock.h"

//------------------------------------------------------------------------------

using testing::EndsWith;
using testing::Eq;
using testing::HasSubstr;
using testing::MatchesRegex;
using testing::StrEq;
using testing::Test;

//------------------------------------------------------------------------------

class OptionsTest : public Test
{
    public:
        OptionsTest()
        {
        }

    protected:
        virtual void SetUp()
        {
            output.str(std::string());
            error.str(std::string());
        }

        virtual void TearDown()
        {
        }

        Options options_;
};

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReportErrorIfNoOptionsGiven)
{
    char *argv[] = { "appname" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_FALSE(result);

    std::string str = error.str();
    ASSERT_THAT(str, HasSubstr("appname"));
    ASSERT_THAT(str, EndsWith("\n"));
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnFilenamesWithLongArgs)
{
    char *argv[] = {
        "appname", "--input-filename", "test.mp3",
        "--output-filename", "test.dat"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getInputFilename(), StrEq("test.mp3"));
    ASSERT_THAT(options_.getOutputFilename(), StrEq("test.dat"));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnFilenamesWithShortArgs)
{
    char *argv[] = { "appname", "-i", "test.mp3", "-o", "test.dat" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getInputFilename(), StrEq("test.mp3"));
    ASSERT_THAT(options_.getOutputFilename(), StrEq("test.dat"));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingInputFilename)
{
    char *argv[] = { "appname", "-i", "-o", "test.dat" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_FALSE(result);

    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingOutputFilename)
{
    char *argv[] = { "appname", "-i", "test.mp3", "-o" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_FALSE(result);

    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnDefaultOptions)
{
    char *argv[] = { "appname", "-i", "test.mp3", "-o", "test.dat" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getStartTime(), Eq(0.0));
    ASSERT_FALSE(options_.hasEndTime());
    ASSERT_THAT(options_.getEndTime(), Eq(0.0));
    ASSERT_FALSE(options_.hasSamplesPerPixel());
    ASSERT_THAT(options_.getSamplesPerPixel(), Eq(256));
    ASSERT_THAT(options_.getBits(), Eq(16));
    ASSERT_THAT(options_.getImageWidth(), Eq(800));
    ASSERT_THAT(options_.getImageHeight(), Eq(250));
    ASSERT_TRUE(options_.getRenderAxisLabels());
    ASSERT_FALSE(options_.getHelp());
    ASSERT_FALSE(options_.getVersion());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnStartTimeWithLongArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--start", "1234.5"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getStartTime(), Eq(1234.5));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnStartTimeWithShortArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-s", "1234.5"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getStartTime(), Eq(1234.5));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfInvalidStartTime)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-s", "invalid"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingStartTime)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-s"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnEndTimeWithLongArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--end", "1234.5"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_TRUE(options_.hasEndTime());
    ASSERT_THAT(options_.getEndTime(), Eq(1234.5));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnEndTimeWithShortArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-e", "1234.5"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_TRUE(options_.hasEndTime());
    ASSERT_THAT(options_.getEndTime(), Eq(1234.5));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfInvalidEndTime)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-e", "invalid"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingEndTime)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-e"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(options_.hasEndTime());

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnDefaultEndTime)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_FALSE(options_.hasEndTime());
    ASSERT_THAT(options_.getEndTime(), Eq(0.0));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnWidthWithLongArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--width", "12345"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getImageWidth(), Eq(12345));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnWidthWithShortArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-w", "12345"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getImageWidth(), Eq(12345));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfInvalidWidth)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-w", "invalid"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingWidth)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-w"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnHeightWithLongArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--height", "23456"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getImageHeight(), Eq(23456));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnHeightWithShortArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-h", "23456"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getImageHeight(), Eq(23456));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfInvalidHeight)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-h", "invalid"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingHeight)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-h"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnZoomWithLongArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--zoom", "1000"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_TRUE(options_.hasSamplesPerPixel());
    ASSERT_THAT(options_.getSamplesPerPixel(), Eq(1000));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnZoomWithShortArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-z", "23456"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_TRUE(options_.hasSamplesPerPixel());
    ASSERT_THAT(options_.getSamplesPerPixel(), Eq(23456));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnDefaultZoomOption)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_FALSE(options_.hasSamplesPerPixel());
    ASSERT_THAT(options_.getSamplesPerPixel(), Eq(256));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfInvalidZoom)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-z", "invalid"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfZoomValueTooLarge)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-z", "2147483648" // INT_MAX + 1
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingZoom)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-z"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnPixelsPerSecond)
{
    char* argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--pixels-per-second", "200"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_TRUE(options_.hasPixelsPerSecond());
    ASSERT_THAT(options_.getPixelsPerSecond(), Eq(200));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnDefaultPixelsPerSecondOption)
{
    char* argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_FALSE(options_.hasPixelsPerSecond());
    ASSERT_THAT(options_.getPixelsPerSecond(), Eq(100));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfInvalidPixelsPerSecond)
{
    char* argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--pixels-per-second", "invalid"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingPixelsPerSecond)
{
    char* argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--pixels-per-second"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfPixelsPerSecondValueTooLarge)
{
    char* argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--pixels-per-second", "2147483648" // INT_MAX + 1
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnBitsWithLongArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--bits", "8"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getBits(), Eq(8));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnBitsWithShortArg)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-b", "16"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_TRUE(result);

    ASSERT_THAT(options_.getBits(), Eq(16));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfBitsInvalid)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "-b", "3"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);

    ASSERT_THAT(options_.getBits(), Eq(3));

    ASSERT_TRUE(output.str().empty());
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfInvalidBits)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--bits", "invalid"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfMissingBits)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat", "--bits"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnDefaultBitsOption)
{
    char *argv[] = {
        "appname", "-i", "test.mp3", "-o", "test.dat"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);
    ASSERT_TRUE(result);

    ASSERT_FALSE(options_.hasBits());
    ASSERT_THAT(options_.getBits(), Eq(16));

    ASSERT_TRUE(output.str().empty());
    ASSERT_TRUE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisableAxisLabelRendering)
{
    char *argv[] = {
        "appname", "-i", "test.dat", "-o", "test.png", "--no-axis-labels"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_TRUE(result);
    ASSERT_TRUE(error.str().empty());

    ASSERT_FALSE(options_.getRenderAxisLabels());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldEnableAxisLabelRendering)
{
    char *argv[] = {
        "appname", "-i", "test.dat", "-o", "test.png", "--with-axis-labels"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_TRUE(result);
    ASSERT_TRUE(error.str().empty());

    ASSERT_TRUE(options_.getRenderAxisLabels());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldEnableAxisLabelRenderingByDefault)
{
    char *argv[] = {
        "appname", "-i", "test.dat", "-o", "test.png"
    };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_TRUE(result);
    ASSERT_TRUE(error.str().empty());

    ASSERT_TRUE(options_.getRenderAxisLabels());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnHelpFlag)
{
    char *argv[] = { "appname", "--help" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_TRUE(result);
    ASSERT_TRUE(options_.getHelp());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnVersionFlagWithLongArg)
{
    char *argv[] = { "appname", "--version" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_TRUE(result);
    ASSERT_TRUE(options_.getVersion());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldReturnVersionFlagWithShortArg)
{
    char *argv[] = { "appname", "-v" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_TRUE(result);
    ASSERT_TRUE(options_.getVersion());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfUnknownLongArg)
{
    char *argv[] = { "appname", "--unknown" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldDisplayErrorIfUnknownShortArg)
{
    char *argv[] = { "appname", "-u" };

    bool result = options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    ASSERT_FALSE(result);
    ASSERT_FALSE(error.str().empty());
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldOutputVersionInfo)
{
    std::ostringstream stream;
    options_.showVersion(stream);

    std::string str = stream.str();
    ASSERT_THAT(str, MatchesRegex("^AudioWaveform v[0-9]+\\.[0-9]+\\.[0-9]+\n$"));
}

//------------------------------------------------------------------------------

TEST_F(OptionsTest, shouldOutputUsageInfo)
{
    char *argv[] = { "appname" };

    // Must call parseCommandLine first...
    options_.parseCommandLine(ARRAY_LENGTH(argv), argv);

    std::ostringstream stream;
    options_.showUsage(stream);

    std::string str = stream.str();
    ASSERT_THAT(str, HasSubstr("appname"));
}

//------------------------------------------------------------------------------
