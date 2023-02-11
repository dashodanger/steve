#include "ConfigJson.h"

using namespace steve;

template <class T>
void parse_number(json_value_s* json_value, T& target) {
  if(const json_number_s* json_number = json_value_as_number(json_value)) {
    target = atoi(json_number->number);
  } else {
  }
}

template <class T>
std::shared_ptr<T> find_item(std::vector<std::shared_ptr<T>>& items, const std::string& name) {
  for(auto& candidate : items) {
    if(candidate->name == name) {
      return candidate;
    }
  }

  std::shared_ptr<T> desc(new T);
  items.push_back(desc);
  desc->name = name;
  return desc;
}

void ConfigJson::parse_file(const char* filepath) {
  FILE* file = fopen(filepath, "rb");
  fseek(file, 0, SEEK_END);
  const size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* file_data = (char*)malloc(file_size);
  fread(file_data, 1, file_size, file);

  parse_buffer(file_data, file_size);

  free(file_data);
}

void ConfigJson::parse_buffer(const char* buffer, size_t size) {
  json_parse_result_s parse_result;
  json_value_s* root = json_parse_ex(buffer, size, 0, nullptr, nullptr, &parse_result);

  if(const json_object_s* root_object = json_value_as_object(root)) {
    for(const json_object_element_s* element = root_object->start; element != nullptr; element = element->next) {
      if(!strcmp(element->name->string, "min_tempo")) {
        parse_number(element->value, min_tempo);
      } else if(!strcmp(element->name->string, "max_tempo")) {
        parse_number(element->value, max_tempo);
      } else if(!strcmp(element->name->string, "chords")) {
        parse_chords(json_value_as_object(element->value));
      } else if(!strcmp(element->name->string, "scales")) {
        parse_scales(json_value_as_object(element->value));
      } else if(!strcmp(element->name->string, "instruments")) {
        parse_instruments(json_value_as_object(element->value));
      } else {
      }
    }
  } else {
  }

  free(root);
}

void ConfigJson::parse_chords(const json_object_s* chords_object) {
  for(const json_object_element_s* chord_element = chords_object->start; chord_element != nullptr; chord_element = chord_element->next) {
    std::shared_ptr<ChordDescription> desc = find_item(_chords, chord_element->name->string);
    if(const json_object_s* chord_object = json_value_as_object(chord_element->value)) {
      parse_chord(chord_object, *desc);
    } else {
    }
  }
}

void ConfigJson::parse_chord(const json_object_s* chord_object, ChordDescription& desc) {
  for(const json_object_element_s* chord_attribute = chord_object->start; chord_attribute != nullptr; chord_attribute = chord_attribute->next) {
    const char* chord_attribute_name = chord_attribute->name->string;
    if(!strcmp(chord_attribute_name, "suffix")) {
      if(const json_string_s* chord_suffix = json_value_as_string(chord_attribute->value)) {
        desc.suffix = chord_suffix->string;
      } else {
      }
    } else if(!strcmp(chord_attribute_name, "tones")) {
      if(const json_array_s* chord_tones = json_value_as_array(chord_attribute->value)) {
        for(json_array_element_s* element = chord_tones->start; element != nullptr; element = element->next) {
          if(const json_number_s* tone = json_value_as_number(element->value)) {
            desc.tones |= 1 << atoi(tone->number);
          } else {
          }
        }
      } else {
      }
    }
  }
}

void ConfigJson::parse_scales(const json_object_s* scales_object) {
  for(const json_object_element_s* scale_element = scales_object->start; scale_element != nullptr; scale_element = scale_element->next) {
    std::shared_ptr<ScaleDescription> desc = find_item(_scales, scale_element->name->string);
    if(const json_object_s* scale_object = json_value_as_object(scale_element->value)) {
      parse_scale(scale_object, *desc);
    }
  }
}

void ConfigJson::parse_scale(const json_object_s* scale_object, ScaleDescription& desc) {
  for(const json_object_element_s* scale_attribute = scale_object->start; scale_attribute != nullptr; scale_attribute = scale_attribute->next) {
    if(!strcmp(scale_attribute->name->string, "tones")) {
      if(const json_array_s* scale_tones = json_value_as_array(scale_attribute->value)) {
        for(json_array_element_s* element = scale_tones->start; element != nullptr; element = element->next) {
          if(const json_number_s* tone = json_value_as_number(element->value)) {
            desc.tones |= 1 << atoi(tone->number);
          } else {
          }
        }
      } else {
      }
    }
  }
}

void ConfigJson::parse_instruments(const json_object_s* instruments_object) {
  for(const json_object_element_s* instrument_element = instruments_object->start; instrument_element != nullptr; instrument_element = instrument_element->next) {
    std::shared_ptr<Instrument> desc = find_item(_instruments, instrument_element->name->string);
    if(const json_object_s* instrument_object = json_value_as_object(instrument_element->value)) {
      parse_instrument(instrument_object, *desc);
    }
  }
}
void ConfigJson::parse_instrument(const json_object_s* instrument_object, Instrument& desc) {
  for(const json_object_element_s* instrument_attribute = instrument_object->start; instrument_attribute != nullptr; instrument_attribute = instrument_attribute->next) {
    if(!strcmp(instrument_attribute->name->string, "index")) {
      parse_number(instrument_attribute->value, desc.midi_id);
    } else if(!strcmp(instrument_attribute->name->string, "min_tone")) {
      parse_number(instrument_attribute->value, desc.min_tone);
    } else if(!strcmp(instrument_attribute->name->string, "max_tone")) {
      parse_number(instrument_attribute->value, desc.max_tone);
    }
  }
}
