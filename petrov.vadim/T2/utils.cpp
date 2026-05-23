#include "utils.hpp"

petrov::IOguard::IOguard(std::basic_ios< char >& s):
  s_(s),
  precision_(s.precision()),
  width_(s.width()),
  flags_(s.flags()),
  fill_(s.fill())
{}

petrov::IOguard::~IOguard()
{
  s_.precision(precision_);
  s_.width(width_);
  s_.flags(flags_);
  s_.fill(fill_);
}

std::istream& petrov::operator>>(std::istream& in, DelimeterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  in >> c;
  if (c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& petrov::operator>>(std::istream& in, CharIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c;
  in >> c;
  if (in && c != '\'')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  in >> dest.ref;
  if (!in)
  {
    return in;
  }

  char x;
  in >> x;
  if (in && x != '\'')
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& petrov::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string label;
  in >> label;
  if (in && label != dest.ref)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& petrov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  return std::getline(in >> DelimeterIO{ '"' }, dest.ref, '"');
}

std::istream& petrov::operator>>(std::istream& in, DobleLitIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  double d = 0.0;
  char sf = '0';
  in >> d >> sf;
  if (!in || (sf != 'd' && sf != 'D'))
  {
    in.setstate(std::ios::failbit);
  }

  dest.ref;
  return in;
}

std::istream& petrov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  bool gk1 = false, gk2 = false, gk3 = false;
  DataStruct input{};
  std::string label;

  in >> DelimeterIO{'('} >> DelimeterIO{':'};
  if (!in)
  {
    return in;
  }

  while (in && in.peek() != ')')
  {
    in >> LabelIO{label};
    if (!in)
    {
      break;
    }

    if (label == "key1")
    {
      in >> DobleLitIO{input.key1};
      if (in)
      {
        gk1 = true;
      }
    }

    else if (label == "key2")
    {
      in >> CharIO{input.key2};
      if (in)
      {
        gk2 = true;
      }
    }

    else if (label == "key3")
    {
      in >> StringIO{input.key3};
      if (in)
      {
        gk3 = true;
      }
    }

    else
    {
      break;
    }
    in >> DelimeterIO{':'};
  }

  in >> DelimeterIO{')'};
  if (in && gk1 && gk2 && gk3)
  {
    dest = input;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  
  return in;
}

std::ostream& petrov::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  IOguard guard(out);
  out << "(:key1" << std::fixed << std::setprecision(1) << data.key1 << "d:";
  out << "key2 '" << data.key2 << "':";
  out << "key3 \"" << data.key3 << "\":)";

  return out;
}

