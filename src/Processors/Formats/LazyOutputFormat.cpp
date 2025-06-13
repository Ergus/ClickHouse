#include <IO/NullWriteBuffer.h>
#include <Processors/Formats/LazyOutputFormat.h>
#include <Processors/Port.h>
#include <Processors/Transforms/AggregatingTransform.h>

#include <Profiler.hpp>

namespace DB
{

NullWriteBuffer LazyOutputFormat::out;

LazyOutputFormat::LazyOutputFormat(const Block & header)
: IOutputFormat(header, out), queue(2)
{
}


void LazyOutputFormat::consume(Chunk chunk)
{
    INSTRUMENT_FUNCTION("LazyOutputFormat::consume")
    (void)(queue.emplace(std::move(chunk)));
}

Chunk LazyOutputFormat::getChunk(UInt64 milliseconds)
{
    INSTRUMENT_FUNCTION("LazyOutputFormat::getChunk")
    if (isFinished())
        return {};

    Chunk chunk;
    if (milliseconds)
    {
        if (!queue.tryPop(chunk, milliseconds))
            return {};
    }
    else
    {
        if (!queue.pop(chunk))
            return {};
    }

    if (chunk)
        info.update(chunk.getNumRows(), chunk.allocatedBytes());

    return chunk;
}

Chunk LazyOutputFormat::getTotals()
{
    return std::move(totals);
}

Chunk LazyOutputFormat::getExtremes()
{
    return std::move(extremes);
}

void LazyOutputFormat::setRowsBeforeLimit(size_t rows_before_limit)
{
    info.setRowsBeforeLimit(rows_before_limit);
}

void LazyOutputFormat::setRowsBeforeAggregation(size_t rows_before_aggregation)
{
    info.setRowsBeforeAggregation(rows_before_aggregation);
}

}
