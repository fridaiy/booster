

#include "PartList.hpp"

namespace booster { namespace web { namespace mime { namespace multipart {

PartList::PartList(const booster::String& boundary)
  : Multipart(boundary)
  , m_readIteratorInitialized(false)
{}

PartList::PartList(const Headers& requestHeaders)
  : Multipart(parseBoundaryFromHeaders(requestHeaders))
  , m_readIteratorInitialized(false)
{
  if(!getBoundary()) {
    throw std::runtime_error("[booster::web::mime::multipart::PartList::PartList]: Error. No 'boundary' value found in headers.");
  }
}

std::shared_ptr<PartList> PartList::createSharedWithRandomBoundary(v_int32 boundarySize) {
  auto boundary = generateRandomBoundary(boundarySize);
  return std::make_shared<PartList>(boundary);
}

std::shared_ptr<Part> PartList::readNextPart(async::Action& action) {
  if(!m_readIteratorInitialized) {
    m_readIteratorInitialized = true;
    m_iterator = m_parts.begin();
  }
  if(m_iterator == m_parts.end()) {
    return nullptr;
  }
  return *m_iterator ++;
}

void PartList::writeNextPart(const std::shared_ptr<Part>& part, async::Action& action) {

  if(part->getName()) {
    m_namedParts[part->getName()].push_back(part);
  }

  m_parts.push_back(part);

}

std::shared_ptr<Part> PartList::getNamedPart(const booster::String& name) {

  auto it = m_namedParts.find(name);
  if(it != m_namedParts.end()) {
    return it->second.front();
  }

  return nullptr;

}

std::list<std::shared_ptr<Part>> PartList::getNamedParts(const booster::String& name) {

  auto it = m_namedParts.find(name);
  if(it != m_namedParts.end()) {
    return it->second;
  }

  return std::list<std::shared_ptr<Part>>{};

}

const std::list<std::shared_ptr<Part>>& PartList::getAllParts() {
  return m_parts;
}

v_int64 PartList::count() {
  return static_cast<v_int64>(m_parts.size());
}

}}}}
