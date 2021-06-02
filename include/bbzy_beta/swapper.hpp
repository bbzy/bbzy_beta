#pragma once
namespace bbzy
{
namespace swapper
{
namespace detail
{
template<class CurFieldT>
void swapObjects(void* curObj, void* otherObj, CurFieldT* curField)
{
    std::ptrdiff_t diff = reinterpret_cast<uint8_t*>(curField) - reinterpret_cast<uint8_t*>(curObj);
    CurFieldT* otherField = reinterpret_cast<CurFieldT*>(reinterpret_cast<uint8_t*>(otherObj) + diff);
    std::swap(*curField, *otherField);
}

}

/**
 * Swap fields of @curObj and @otherObj
 * @param curObj first object
 * @param otherObj secont object
 * @param curFields field pointers of @curObj
 */
template<class ObjT, class... CurFieldTs>
void swapObjects(ObjT& curObj, ObjT& otherObj, CurFieldTs* ... curFields)
{
    auto _ = {(detail::swapObjects(&curObj, &otherObj, curFields), 0)...};
}

}
}